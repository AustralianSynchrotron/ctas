#include "matrix.world.h"
#include "parallel.world.h"
#include "external.world.h"

using namespace std;


const string Segment::modname = "Segment";

Segment::Segment(const string & str) : Segment() {
  if (!str.size())
    return;
  static const string spltChars = "+-:";
  deque<string> subs = splits(str, spltChars);
  if (1==subs.size())
    subs.push_back("");
  if ( 2 != subs.size() || ( ! subs[0].empty() && ! parse_num(subs[0], &_from) )
                        || ( ! subs[1].empty() && ! parse_num(subs[1], &_to) ) )
    throw_error(modname, "Could not parse string \""+str+"\"."
                         " Must follow pattern [UINT][<"+spltChars+">UINT].");
  const size_t splt = str.find_first_of(spltChars);
  const char spltUsed = splt==string::npos ? '-' : str[splt];
  if ('+'==spltUsed)
    _to += _from;
  else if ('-'==spltUsed)
    _to *= -1;
  else if (':'!=spltUsed)
    throw_bug(modname + " parsing of \"" + str + "\".");
  check_throw();
}

void Segment::check_throw() {
  if (_from < 0)
    throw_error(modname, "Negative starting point ("+toString(_to)+").");
  if (_to>0 && _to<=_from)
    throw_error(modname,
      "End ("+toString(_to)+") is less or equal than start ("+toString(_from)+").");
}


ssize_t Segment::size(size_t orgsz) const {
  ssize_t sz = _to + ( _to > 0 ? 0 : orgsz ) - _from ;
  if (sz<=0)
    throw_error("Range size", "Negative size for range ["+toString(_from)+","+toString(_to)+"]"
                + ( _to>0 ? "" : (" and original size "+toString(orgsz)) ) + "." );
  if ( _to>0 && orgsz>0 && orgsz < _to )
    throw_error("Range size", "Range ["+toString(_from)+","+toString(_to)+"]"
                + " is larger than original size "+toString(orgsz)+ "." );
  return sz;
}


template<int Dim>
const string Crop<Dim>::modname = toString(Dim)+"D-segment";


template<int Dim>
Crop<Dim>::Crop(const string & str) {
  if (str.empty())
    return;
  deque<string> subs = split(str, ",");
  while ( Dim > subs.size() ) // adds missing dimensions.
    subs.push_back("");
  if ( Dim != subs.size() )
    throw_error(modname, "Could not parse string \""+str+"\". Must contain " + toString(Dim)
                         + " comma-delimited substrings, while " + toString(subs.size()) + " found.");
  for (uint curD=0; curD<Dim; ++curD)
    // Dim-1-curD here is to reflect the fact that natural x,y,z... in matrix notaition has inverted order
    this->operator()(Dim-1-curD) = Segment(subs[curD]);
}

template<int Dim>
Shape<Dim> Crop<Dim>::shape(const Shape<Dim> & ish) const {
  Shape<Dim> osh;
  for (uint curD=0; curD<Dim; ++curD)
    osh(curD) = (*this)(curD).size(ish(curD));
  return osh;
}

template<int Dim>
ArrayF<Dim> Crop<Dim>::apply(const ArrayF<Dim> & iarr) const {
  if (!bool(*this))
    return iarr;
  const Shape<Dim> osh = shape(iarr.shape());
  blitz::TinyVector<size_t,Dim> ubound;
  blitz::TinyVector<size_t,Dim> lbound;
  for (uint curD=0; curD<Dim; ++curD) {
    ubound[curD] = (*this)[curD].begin();
    lbound[curD] = ubound[curD] + osh[curD] - 1;
  }
  return iarr(blitz::RectDomain<Dim>(ubound, lbound));
}


template class Crop<1>;
template class Crop<2>;
template class Crop<3>;


const string CropOptionDesc = "Each SEG describes the segment of the corresponding axis."
  " SEG format is [UINT][[:+-]UINT], where fist number is the starting point of the segment"
  " and second one can be (depending on the delimiter)"
  " ':' - end point, '+' - length of the segment or '-' - crop from the end."
;


static const CLprogram & matrixOCLprogram(const cl_context cont=CL_context()) {
  if (!CL_isReady())
    throw_error("MatrixCL", "OpenCL is not functional.");
  static unordered_map<cl_context, CLprogram> progs;
  static pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
  static const string oclsrc = {
          #include "matrix.cl.includeme"
  };
  if (!progs.count(cont)) {
    pthread_mutex_lock(&locker);
    if (!progs.count(cont)) // check again to make sure it was not created before locking
      progs.try_emplace(cont, oclsrc, cont);
    pthread_mutex_unlock(&locker);
  }
  return progs.at(cont);
};



class BinnProc::ForCLdev {

  static const std::string modname;
  const Shape<2> ish;
  const Binn<2> rbnn;
  const Shape<2> osh;
  CLenv & cl;
  CLkernel kernelBinn;
  CLmem clinarr;
  CLmem cloutarr;
  pthread_mutex_t locker;
  int useCounter=0;

public:

  ForCLdev(CLenv & cl, const Shape<2> & _ish, const Binn<2> & _rbnn)
    : rbnn(_rbnn)
    , ish(_ish)
    , osh(rbnn.shape(ish))
    , cl(cl)
    , locker(PTHREAD_MUTEX_INITIALIZER)
  {
    if (!rbnn)
      throw_bug(modname+" Not intended for trivial binnings.");
  }

  ~ForCLdev(){
    pthread_mutex_destroy(&locker);
#ifdef DEBUGCTAS
    cout << "DBGCTAS: BinnProc::ForCLdev " << toString("%p", this) << " : " << useCounter << "\n";
#endif // DEBUGCTAS
  }

  bool apply(const Map & imap, Map & omap) {

    if( pthread_mutex_trylock(&locker) )
      return false;
    ++useCounter;

    bool done = false;
    try {

      if (!kernelBinn) { // OpenCL infrastructure is created on first call.
        kernelBinn(matrixOCLprogram(cl.cont), "binn2");
        clinarr(clAllocArray<float>(size(ish), CL_MEM_READ_ONLY, cl.cont));
        cloutarr(clAllocArray<float>(size(osh), CL_MEM_WRITE_ONLY, cl.cont));
        if ( ! kernelBinn || ! clinarr() || ! cloutarr() )
          throw 1;
        kernelBinn.setArg(0, clinarr());
        kernelBinn.setArg(1, cloutarr());
        kernelBinn.setArg(2, (cl_int) rbnn(1));
        kernelBinn.setArg(3, (cl_int) rbnn(0));
        kernelBinn.setArg(4, (cl_int) ish(1));
        kernelBinn.setArg(5, (cl_int) ish(0));
      }

      cl_command_queue que = clenv(kernelBinn.context()).que;
      blitz2cl<cl_float>(imap, clinarr(), que);
      kernelBinn.exec(osh, que);
      cl2blitz(cloutarr(), omap, que);
      done = true;

    } catch (...) {}

    pthread_mutex_unlock(&locker);
    if (!done)
      throw_error(modname, "Failed to binn.");
    return done;

  }

};
const string BinnProc::ForCLdev::modname = "BinnOCL";


struct BinnProc::Accumulator::BinnAcc {

  const Shape<2> mish;
  size_t bn;
  ssize_t odx;
  unsigned cnt;
  CLmem resmem;
  CLmem addmem;
  pthread_mutex_t locker;
  CLprogram binnProgram;
  CLkernel addKernel;
  CLkernel divKernel;

  BinnAcc(Shape<2> _mish, ssize_t _bn)
    : mish(_mish)
    , bn(abs(_bn))
    , odx(-1)
    , cnt(0)
    , locker(PTHREAD_MUTEX_INITIALIZER)
  {
    if (!bn)
      throw_bug("CLacc: zero binning. Here must be size.");
    if (bn==1)
      return;

    static const string oclsrc = {
      #include "../common/matrix.cl.includeme"
    };
    binnProgram(oclsrc);
    resmem(clAllocArray<float>(size(mish)));
    addmem(clAllocArray<float>(size(mish), CL_MEM_READ_ONLY) );
    addKernel(binnProgram, "addToSecond");
    addKernel.setArg(0, addmem());
    addKernel.setArg(1, resmem());
    fillClArray<float>(resmem(), size(mish), 0);
    divKernel(binnProgram, "multiplyArray");
    divKernel.setArg(0, resmem());
    divKernel.setArg(1, (float)1.0/bn);

  }

  ~BinnAcc() {
    pthread_mutex_destroy(&locker);
  }

  unsigned int addme (Map & nmap) {
    if (bn==1)
      return 0;
    if (nmap.shape() != mish)
      throw_error("Sum on CL", "Wrong input image shape.");
    pthread_mutex_lock(&locker);
    blitz2cl<cl_float>(nmap, addmem());
    addKernel.exec(size(mish));
    cnt++;
    if (cnt==bn)
      getme(nmap);
    pthread_mutex_unlock(&locker);
    return cnt;
  }

  unsigned int getme (Map & nmap) {
    const unsigned int toRet = odx;
    if ( ! cnt || bn==1 )
      return toRet;
    nmap.resize(mish);
    if (cnt != bn) // will happen only for the last incomplete slice
      divKernel.setArg(1, (float)1.0/cnt);
    divKernel.exec(size(mish));
    cl2blitz(resmem(), nmap);
    if (cnt == bn)
      fillClArray<float>(resmem(), size(mish), 0);
    cnt=0;
    odx=-1;
    return toRet;
  }


};



BinnProc::Accumulator::Accumulator(const Shape<2> & ish, ssize_t bn)
  : guts(new BinnAcc(ish, bn))
{
  if (!guts)
    throw_error("BinnAcc", "Failed to create.");
}

BinnProc::Accumulator::~Accumulator() {
  if (guts)
    delete guts;
}

unsigned int
BinnProc::Accumulator::addme(Map & nmap) {
  if (!guts)
    throw_bug("BinnAcc is a ghost.");
  return guts->addme(nmap);
}

unsigned int
BinnProc::Accumulator::getme(Map & nmap) {
  if (!guts)
    throw_bug("BinnAcc is a ghost.");
  return guts->getme(nmap);
}

ssize_t
BinnProc::Accumulator::index() const {
  if (!guts)
    throw_bug("BinnAcc is a ghost.");
  return guts->odx;
}

void
BinnProc::Accumulator::index(ssize_t idx) {
  if (!guts)
    throw_bug("BinnAcc is a ghost.");
  guts->odx = idx;
}




BinnProc::BinnProc(const Shape<2> & ish, const Binn<2> & bnn)
  : bnn(bnn)
  , rbnn( [&ish,bnn](){
      Binn<2>toRet;
      for (int dim=0; dim<2; dim++)
        toRet(dim) = bnn(dim) ? abs(bnn(dim)) : ish(dim) ;
      return toRet;
    }())
  , ish(ish)
  , osh(bnn.shape(ish))
  , envs(_envs)
{
  if (!size(ish)) {
    warn(modname, "Zero input size to binn.");
    return;
  }
  if (!size(osh)) {
    warn(modname, "Zero result of binning"
                  " ("+toString(ish)+") by (" + toString(bnn) + ").");
    return;
  }
  if (bnn.isTrivial()) // Trivial binning. No need OpenCL.
    return;
  if (!CL_isReady())
    warn(modname, "OpenCL is not functional.");
  else
    for (CLenv & env : clenvs)
      try{ _envs.emplace_back( new ForCLdev(env, ish, rbnn) ); } catch(...) {}
}

BinnProc::BinnProc(const BinnProc & other)
  : bnn(other.bnn)
  , rbnn(other.rbnn)
  , ish(other.ish)
  , osh(other.osh)
  , envs(other.envs)
{}

BinnProc::~BinnProc() {
  for (ForCLdev * env : _envs)
    if (env)
      delete env;
}

Map BinnProc::apply(const Map & imap, Map & tmap) const {

  if (!bnn)
    return imap;
  if (imap.shape() != ish)
    throw_error(modname, "Missmatch of input shape ("+toString(imap.shape())+")"
                         " with expected ("+toString(ish)+").");
  if (!size(ish) || !size(osh))
    return imap;
  if (bnn.isTrivial())
    return bnn.apply(imap);

  Map fimap(imap);
  for (uint curD=0; curD<2; ++curD) // flipping
    if (bnn(curD) < 0)
      fimap.reverseSelf(curD);

  if (!tmap.size())
    tmap.resize(osh);
  if ( tmap.shape() != osh )
    throw_error(modname, "Missmatch of interim array shape ("+toString(tmap.shape())+")"
                         " with expected ("+toString(osh)+")."
                         " It is assumed to be empty or ready before getting here." );
  for (ForCLdev * env : envs)
    if (env->apply(fimap,tmap))
      return tmap;
  // Do on CPU
  for (ssize_t y=0 ; y<osh(0) ; ++y) {
    const int bby = min(ish(0), (y+1)*rbnn(0)) - y*rbnn(0);
    for (ssize_t x=0 ; x<osh(1) ; ++x) {
      const int bbx = min(ish(1), (x+1)*rbnn(1)) - x*rbnn(1);
      float & val = tmap(y,x);
      val = 0;
      for (ssize_t cy = 0 ; cy < bby ; cy++) {
        const ssize_t yy = y*rbnn(0) + cy;
        for (ssize_t cx = 0 ; cx < bbx ; cx++)
          val += fimap(yy, x*rbnn(1) + cx);
      }
      val /= bbx*bby;
    }
  }
  return tmap;
}

const string BinnProc::modname = "BinnProc";





template<int Dim>
Binn<Dim>::Binn(const string & str) {
  if (str.empty())
    return;
  const deque<string> subs = split(str, ",");
  int sbnn;
  if ( 1 == subs.size() && !parse_num(str, &sbnn ) )
    throw_error(modname, "Could not parse string \""+str+"\" as integer number.");
  if ( 1 != subs.size()  &&  Dim != subs.size() )
    throw_error(modname, "Could not parse string \""+str+"\". Must contain single or " + toString(Dim)
                         + " comma-delimited integers, while " + toString(subs.size()) + " found.");
  for (uint curD=0; curD<Dim; ++curD) {
    if ( Dim == subs.size() ) {
      const string & curStr = subs.at(curD);
      if (curStr.empty())
        sbnn=1;
      else if ( ! parse_num(curStr, &sbnn ) )
        throw_error(modname, "Could not parse string \""+curStr+"\" as integer number"
                             " for dimension " + toString(curD) + ".");
    }
    // Dim-1-curD here is to reflect the fact that natural x,y,z... in matrix notaition has inverted order
    (*this)(Dim-1-curD) = sbnn ;
  }
}


template<int Dim>
Binn<Dim> Binn<Dim>::flipped() const {
  Binn<Dim> toRet;
  for (int dim=0; dim<Dim; dim++)
    toRet(dim) = abs((*this)(dim));
  return toRet;
}


template<int Dim>
Shape<Dim> Binn<Dim>::shape(const Shape<Dim> & ish) const {
  Shape<Dim> osh;
  for (uint curD=0; curD<Dim; ++curD)
    osh(curD) = binnOne( ish(curD), (*this)(curD) );
  return osh;
}


template<> ArrayF<1> Binn<1>::subapply( const ArrayF<1> & iarr, ArrayF<1> & oarr) const {
  throw_bug("1D Binning is not implemented yet.");
  return oarr;
  //oarr.resize(bnn.apply(iarr.shape()));
  // TODO
}

template<> ArrayF<2> Binn<2>::subapply( const ArrayF<2> & iarr, ArrayF<2> & oarr) const {
  return BinnProc(iarr.shape(), flipped()).apply(iarr, oarr);
}

template<> ArrayF<3> Binn<3>::subapply( const ArrayF<3> & iarr, ArrayF<3> & oarr) const {

  const Shape<3> ish = iarr.shape();
  const Shape<3> osh = oarr.shape();
  Binn<3>rbnn;
  for (int dim=0; dim<2; dim++)
    rbnn(dim) = (*this)(dim) ? abs((*this)(dim)) : ish(dim) ;

  if (!CL_isReady())
    throw_error(modname, "OpenCL is not functional.");
  CLprogram binnProgram(matrixOCLprogram());

  try {

    CLmem clinarr(blitz2cl<cl_float>(iarr, CL_MEM_READ_ONLY));
    CLmem cloutarr(clAllocArray<float>(oarr.size(), CL_MEM_WRITE_ONLY));
    CLkernel kernelBinn3(binnProgram, "binn3");

    kernelBinn3.setArg(0, clinarr());
    kernelBinn3.setArg(1, cloutarr());
    kernelBinn3.setArg(2, (cl_int) rbnn(2));
    kernelBinn3.setArg(3, (cl_int) rbnn(1));
    kernelBinn3.setArg(4, (cl_int) rbnn(0));
    kernelBinn3.setArg(5, (cl_int) ish(2));
    kernelBinn3.setArg(6, (cl_int) ish(1));
    kernelBinn3.setArg(7, (cl_int) ish(0));

    kernelBinn3.exec(osh);
    cl2blitz(cloutarr(), oarr);

  }  catch (...) { // probably full volume was too big for the gpu

    warn("Binning", "Trying second method.");
    const Shape<2> sish(copyMost<2>(ish));
    const Binn<2> sbnn(copyMost<2>(rbnn));
    const ssize_t zbnn = rbnn(0);
    const Shape<2> sosh(copyMost<2>(osh));
    const size_t sosz(size(osh));

    Map tmpslice(sosh);
    CLmem cltmpslice(clAllocArray<float>(sosz));
    Map outslice(sosh);
    CLmem cloutslice(clAllocArray<float>(sosz));
    CLmem clinslice;
    CLkernel kernelBinn2(binnProgram, "binn2");

    if (sbnn) {
      clinslice(clAllocArray<float>(size(sish), CL_MEM_READ_ONLY));
      kernelBinn2.setArg(0, clinslice());
      kernelBinn2.setArg(1, cltmpslice());
      kernelBinn2.setArg(2, (cl_int) sbnn(1));
      kernelBinn2.setArg(3, (cl_int) sbnn(0));
      kernelBinn2.setArg(4, (cl_int) sish(1));
      kernelBinn2.setArg(5, (cl_int) sish(0));
    }

    CLkernel kernelAddTo(binnProgram, "addToSecond");
    kernelAddTo.setArg(0, cltmpslice());
    kernelAddTo.setArg(1, cloutslice());

    CLkernel kernelMulti(binnProgram, "multiplyArray");
    kernelMulti.setArg(0, cloutslice());
    kernelMulti.setArg(1, (cl_float) zbnn);

    for (int z = 0  ;  z < osh(0)  ;  z++ ) {
      fillClArray(cloutslice(), sosz, 0);
      int cz;
      for (cz=0 ; cz<zbnn && z*zbnn+cz < ish(0) ; cz++) {
        Map inslice(iarr(z*zbnn+cz, all, all));
        if (sbnn) {
          blitz2cl<cl_float>(inslice, clinslice());
          kernelBinn2.exec(sosh);
        } else {
          blitz2cl<cl_float>(inslice, cltmpslice());
        }
        kernelAddTo.exec(sosz);
      }
      if (cz && cz != zbnn) // may happen only once for last slice
        kernelMulti.setArg(1, (cl_float) cz);
      kernelMulti.exec(sosh);
      cl2blitz(cloutslice(), outslice);
      oarr(z, all, all) = outslice;
    }

  }

  return oarr;

}


template<int Dim>
ArrayF<Dim> Binn<Dim>::apply(const ArrayF<Dim> & iarr) const {

  if (!(*this))
    return iarr;
  ArrayF<Dim> fiarr(iarr);
  for (uint curD=0; curD<Dim; ++curD) // flipping
    if ((*this)(curD) < 0)
      fiarr.reverseSelf(curD);
  if (isTrivial())
    return fiarr;

  const Shape<Dim> ish = iarr.shape();
  const Shape<Dim> osh = shape(ish);
  ArrayF<Dim> outarr(osh);
  return subapply(fiarr, outarr);

}


template<int Dim>
const string Binn<Dim>::modname = toString(Dim)+"D-binning";


template class Binn<1>;
template class Binn<2>;
template class Binn<3>;

const string BinnOptionDesc =
  "Same for all dimensions if only one provided."
  " Reverses dimension if negative."
  " Averages over the whole dimension if zero.";





class RotateProc::ForCLdev {

  static const std::string modname;
  const RotateProc * parent;
  CLenv & cl;
  CLkernel kernelRotate;
  CLmem clinarr;
  CLmem cloutarr;
  CLmem clxf;
  CLmem clyf;
  CLmem clflx;
  CLmem clfly;
  pthread_mutex_t locker;
  int useCounter=0;

public:

  ForCLdev(CLenv & cl, const RotateProc * parent)
    : parent(parent)
    , cl(cl)
    , locker(PTHREAD_MUTEX_INITIALIZER)

  {
    if (!parent)
      throw_bug("Zerro pointer to RotateProc");
  }

  ~ForCLdev(){
    pthread_mutex_destroy(&locker);
#ifdef DEBUGCTAS
    cout << "DBGCTAS: RotateProc::ForCLdev " << toString("%p", this) << " : " << useCounter << "\n";
#endif // DEBUGCTAS
  }

  bool apply(const Map & imap, Map & omap, const float bg) {

    if( pthread_mutex_trylock(&locker) )
      return false;
    useCounter++;

    try {

      if (!kernelRotate) { // OpenCL infrastructure is created on first call.
        kernelRotate(matrixOCLprogram(cl.cont), "rotate2");
        clinarr(clAllocArray<cl_float>(size(parent->ish), CL_MEM_READ_ONLY, cl.cont));
        cloutarr(clAllocArray<cl_float>(size(parent->osh), CL_MEM_WRITE_ONLY, cl.cont));
        clxf(blitz2cl<cl_float>(parent->xf, CL_MEM_READ_ONLY, cl.que));
        clyf(blitz2cl<cl_float>(parent->yf, CL_MEM_READ_ONLY, cl.que));
        clflx(blitz2cl<cl_int>(parent->flx, CL_MEM_READ_ONLY, cl.que));
        clfly(blitz2cl<cl_int>(parent->fly, CL_MEM_READ_ONLY, cl.que));
        kernelRotate.setArg(0, clinarr());
        kernelRotate.setArg(1, cloutarr());
        //kernelRotate.setArg(2, (cl_float) 0.0); // BG
        kernelRotate.setArg(3, (cl_int) parent->ish(1));
        kernelRotate.setArg(4, (cl_int) parent->ish(0));
        kernelRotate.setArg(5, (cl_int) parent->osh(1));
        kernelRotate.setArg(6, clxf());
        kernelRotate.setArg(7, clyf());
        kernelRotate.setArg(8, clflx());
        kernelRotate.setArg(9, clfly());
      }

      kernelRotate.setArg(2, (cl_float) bg);
      cl_command_queue que = clenv(kernelRotate.context()).que;
      blitz2cl<cl_float>(imap, clinarr(), que);
      kernelRotate.exec(parent->osh, que);
      cl2blitz(cloutarr(), omap, que);

    } catch (...) {
      pthread_mutex_unlock(&locker);
      throw_error(modname, "Failed operation.");
    }
    pthread_mutex_unlock(&locker);
    return true;

  }

};
const string RotateProc::ForCLdev::modname = "RotateOCL";



RotateProc::RotateProc(const Shape<2> & ish, float ang)
  : ang(ang)
  , ish(ish)
  , osh(shape(ish,ang))
  , envs(_envs)
  , xf(osh)
  , yf(osh)
  , flx(osh)
  , fly(osh)
{

  if (!size(ish)) {
    warn(modname, "Zero input size to rotate.");
    return;
  }
  if (isTrivial()) // 90 step rotation. No need OpenCL.
    return;

  const float
    cosa = cos(-ang), sina = sin(-ang),
    constinx = ( ish(1) + osh(0)*sina - osh(1)*cosa ) / 2.0,
    constiny = ( ish(0) - osh(1)*sina - osh(0)*cosa ) / 2.0;
  xf = i1*cosa - i0*sina + constinx;
  yf = i1*sina + i0*cosa + constiny;
  flx = floor(xf);
  fly = floor(yf);
  xf -= flx;
  yf -= fly;

  if (!CL_isReady())
    warn(modname, "OpenCL is not functional.");
  for (CLenv & env : clenvs)
    try{ _envs.emplace_back( new ForCLdev(env, this) ); } catch(...) {}

}

RotateProc::RotateProc(const RotateProc & other)
  : ang(other.ang)
  , ish(other.ish)
  , osh(other.osh)
  , envs(other.envs)
  , xf(other.xf)
  , yf(other.yf)
  , flx(other.flx)
  , fly(other.fly)
{}

RotateProc::~RotateProc() {
  for (ForCLdev * env : _envs)
    if (env)
      delete env;
}


Shape<2> RotateProc::shape(const Shape<2> & ish, float ang) {
  if (!size(ish))
    return Shape<2>(0l);
  else if ( abs( remainder(ang, M_PI/2) ) < 2.0/diag(ish) ) { // close to a 90-deg step
    if ( ! ( ((int) round(2*ang/M_PI)) % 2 ) )
      return ish;
    else
      return Shape<2>(ish(1),ish(0));
  } else {
    const float cosa = cos(-ang), sina = sin(-ang);
    return Shape<2>( abs(ish(1)*sina) + abs(ish(0)*cosa)
                   , abs(ish(1)*cosa) + abs(ish(0)*sina) );
  }
}


Map RotateProc::apply(const Map & imap, Map & tmap, float bg) const {

  if (imap.shape() != ish)
    throw_error(modname, "Missmatch of input shape ("+toString(imap.shape())+")"
                         " with expected ("+toString(ish)+").");

  if ( isTrivial() ) { // close to a 90-deg step
    const int nof90 = round(2*ang/M_PI);
    if ( ! (nof90%4)  ) // 360deg
      return imap;
    Map toRet(imap);
    if ( ! (nof90%2) ) { //180deg
      toRet.reverseSelf(blitz::firstDim);
      toRet.reverseSelf(blitz::secondDim);
    } else if (  ( nof90 > 0 && (nof90%3) ) || ( nof90 < 0 && ! (nof90%3) ) ) {  // 270deg
      toRet.transposeSelf(blitz::firstDim, blitz::secondDim);
      toRet.reverseSelf(blitz::secondDim);
    } else { // 90deg
      toRet.transposeSelf(blitz::firstDim, blitz::secondDim);
      toRet.reverseSelf(blitz::firstDim);
    }
    return toRet;
  }

  if (!tmap.size())
    tmap.resize(osh);
  if (tmap.shape() != osh)
    throw_error(modname, "Missmatch of interim array shape ("+toString(tmap.shape())+")"
                         " with expected ("+toString(osh)+")."
                         " It is assumed to be empty or ready before getting here." );
  if ( ! isnormal(bg) )
    bg = 0.25 * ( mean(imap(all,0))        + mean(imap(0,all))
                + mean(imap(all,ish(1)-1)) + mean(imap(ish(0)-1,all)) );

  for (ForCLdev * env : envs)
    if (env->apply(imap, tmap, bg))
      return tmap;
  //// Do on CPU
  for ( ssize_t y=0 ; y < osh(0) ; y++) {
    for ( ssize_t x=0 ; x < osh(1) ; x++) {
      const ssize_t & vflx = flx(y,x), vfly = fly(y,x);
      if ( vflx < 1 || vflx >= ish(1)-1 || vfly < 1  || vfly >= ish(0)-1 ) {
        tmap(y,x)=bg;
      } else {
        float v0 = imap(vfly,  vflx) + ( imap(vfly,  vflx+1) - imap(vfly,  vflx) ) * xf(y,x);
        float v1 = imap(vfly+1,vflx) + ( imap(vfly+1,vflx+1) - imap(vfly+1,vflx) ) * yf(y,x);
        tmap(y,x) = v0 + (v1-v0) * yf(y,x);
      }
    }
  }
  return tmap;

}

const string RotateProc::modname = "RotateProc";




MapProc::MapProc(float ang, const Crop<2> & crp, const Binn<2> & bnn, const Shape<2> & ish, float reNAN)
  : ish(ish)
  , rotProc(ish, ang)
  , crp(crp)
  , bnnProc(crp.shape(rotProc.shape()), bnn)
  , osh(bnnProc.shape())
  , reNAN(reNAN)
{}

MapProc::MapProc(const MapProc & other)
  : ish(other.ish)
  , rotProc(other.rotProc)
  , crp(other.crp)
  , bnnProc(other.bnnProc)
  , osh(other.osh)
  , reNAN(other.reNAN)
{}


Map MapProc::apply(const Map & imap) const {
  static const string modname="MapProc";
  if (imap.shape() != ish)
    throw_error(modname, "Missmatch of input shape ("+toString(imap.shape())+")"
                         " with expected ("+toString(ish)+").");
  Map rmap = rotProc.apply(imap, rotmap, reNAN);
  Map cmap = crp.apply(rmap);
  Map bmap = bnnProc.apply(cmap, bnnmap);
  return bmap;
}


Shape<2> MapProc::shape(float ang, const Crop<2> & crp, const Binn<2> & bnn, const Shape<2> & ish) {
  return bnn.shape(crp.shape(RotateProc::shape(ish,ang)));
}



namespace blitz {

static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);

static inline float invert(float x){ return x==0.0 ? 0.0 : 1.0/x ;}
BZ_DECLARE_FUNCTION(invert);

static inline float subzero(float x){ return x < 0.0 ? 0.0 : x ;}
BZ_DECLARE_FUNCTION(subzero);


static inline float thrshld(float v, float av){ return v-av; }
BZ_DECLARE_FUNCTION2(thrshld);

}


Denoiser::Denoiser(const Shape<2> & _sh, int _rad, float _threshold, const Map & _mask)
  : sh(_sh)
  , rad(abs(_rad))
  , thr(_threshold)
  , mask(_mask)
{
  if ( !size(sh) || !rad )
    return;
  const ssize_t mssz=_mask.size();
  if ( mssz && _mask.shape() != sh)
    throw_error("denoiser", "Mask shape ("+toString(_mask.shape())+") does not match"
                            " that of the denoiser("+toString(sh)+").");
  tarr.resize(sh);
  swghts.resize(sh);
  swghts = 0.0;
  int cntr = 0;
  const uint rad2 = rad*rad;
  for (int ii = -rad ; ii <= rad ; ii++)
    for (int jj = -rad ; jj <= rad ; jj++)
      if ( ii*ii + jj*jj <= rad2 ) {
        if (mssz)
          swghts(dstRa(sh, ii, jj)) += mask (srcRa(sh, ii, jj));
        else
          swghts(dstRa(sh, ii, jj)) += 1.0;
        cntr++;
      }
  swghts = invert(swghts);
}


Denoiser::Denoiser(const Denoiser & other)
  : sh(other.sh)
  , tarr(sh)
  , swghts(other.swghts)
  , mask(other.mask)
  , rad(other.rad)
  , thr(other.thr)
{}


void Denoiser::proc(Map & iom) const {

  if ( !size(sh) || !rad )
    return;
  if ( iom.shape() != sh )
    throw_error("denoiser", "Non matching shape of input array.");
  if ( mask.size() &&  mask.shape() != sh )
    throw_error("denoiser", "Non matching shape of the mask.");

  static const uint rad2 = rad*rad;
  tarr=0.0;
  if (mask.size())
    iom *= mask;
  for (int ii = -rad ; ii <= rad ; ii++)
    for (int jj = -rad ; jj <= rad ; jj++)
      if ( ii*ii + jj*jj <= rad2 )
        tarr(dstRa(sh, ii, jj)) += iom (srcRa(sh, ii, jj));
  tarr *= swghts;

  if (thr == 0.0) {
    iom = tarr;
  } else if (thr<0) {
    for (ssize_t ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ssize_t xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 || ( tval != 0.0  &&  abs((ival-tval)/tval) > -thr) )
          ival = tval;
      }
  } else {
    for (ssize_t ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ssize_t xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 ||  abs(ival-tval) > thr)
          ival = tval;
      }
  }

}
