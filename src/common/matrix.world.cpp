#include "matrix.world.h"
#include "parallel.world.h"

using namespace std;


const string Segment::modname = "Segment";

Segment::Segment(const string & str) : _from(0), _to(0) {
  static const string spltChars = "+-:";
  const size_t splt = str.find_first_of(spltChars);
  const string fromS = str.substr(0, splt);
  const string toS = splt==string::npos
    ?  string() : str.substr(splt+1, string::npos);
  size_t scorto;
  try {
    _from = fromS.empty() ? 0 : stoul(fromS);
    scorto = toS.empty() ? 0 : stoul(toS);
  } catch (...) {
    throw_error(modname, "Could not parse string \""+str+"\"."
                         " Must follow pattern [UINT][<"+spltChars+">UINT].");
  }
  switch (splt==string::npos ? '-' : str[splt]) {
    case '+': _to = _from+scorto; break;
    case ':': _to = scorto; break;
    case '-': _to = -scorto; break;
    default: throw_bug(modname + " parsing of \"" + str + "\".");
  }
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
Shape<Dim> Crop<Dim>::apply(const Shape<Dim> & ish) const {
  Shape<Dim> osh;
  for (uint curD=0; curD<Dim; ++curD)
    osh(curD) = (*this)(curD).size(ish(curD));
  return osh;
}

template<int Dim>
ArrayF<Dim> Crop<Dim>::apply(const ArrayF<Dim> & iarr) const {
  if (!bool(*this))
    return iarr;
  const Shape<Dim> osh = apply(iarr.shape());
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






class Binn2D {

public:

  const Binn<2> bnn;
  const Shape<2> ish;
  static cl_program & binnProgram() {
    if (clProgram)
      return clProgram;
    if (!CL_isReady())
      throw_error(modname, "OpenCL is not functional.");
    static const string oclsrc = {
      #include "binn.cl.includeme"
    };
    clProgram = initProgram(oclsrc, clProgram, modname);
    return clProgram;
  };

private:

  const Shape<2> osh;
  static cl_program clProgram; // =0;
  static const string modname;

  class ForCLdev {
    const Shape<2> osh;
    CLenv & cl;
    CLkernel kernelBinn;
    CLmem clinarr;
    CLmem cloutarr;
    pthread_mutex_t locker;
  public:
    ForCLdev(CLenv & cl, const Shape<2> & ish, const Binn<2> bnn)
      : osh(bnn.apply(ish))
      , cl(cl)
      , locker(PTHREAD_MUTEX_INITIALIZER)
    {
      if (!CL_isReady())
        throw_error(modname, "OpenCL is not functional.");
      try {
        kernelBinn(binnProgram(), "binn2");
        clinarr(clAllocArray<float>(size(ish), CL_MEM_READ_ONLY));
        cloutarr(clAllocArray<float>(size(osh), CL_MEM_WRITE_ONLY));
        if ( ! kernelBinn || ! clinarr() || ! cloutarr() )
          throw 1;
        kernelBinn.setArg(0, clinarr());
        kernelBinn.setArg(1, cloutarr());
        kernelBinn.setArg(2, (cl_int) abs(bnn(1)));
        kernelBinn.setArg(3, (cl_int) abs(bnn(0)));
        kernelBinn.setArg(4, (cl_int) ish(1));
        kernelBinn.setArg(5, (cl_int) ish(0));
      } catch (...) {
        kernelBinn.free();
        clinarr.free();
        cloutarr.free();
        throw_error(modname, "Failed to prepare for operation.");
      }
    }

    ~ForCLdev(){
      pthread_mutex_destroy(&locker);
    }

    bool apply(const Map & imap, Map & omap) {
      if ( ! binnProgram() || ! CL_isReady() )
        throw_error(modname, "OpenCL is not functional or binn program has not compiled.");
      if( pthread_mutex_trylock(&locker) )
        return false;
      blitz2cl(imap, clinarr());
      kernelBinn.exec(osh);
      cl2blitz(cloutarr(), omap);
      pthread_mutex_unlock(&locker);
      return true;
    }

  };

  std::list<ForCLdev>  envs;

public:

  Binn2D(const Shape<2> & _ish, const Binn<2> & _bnn)
    : bnn( _bnn(0) ? _bnn(0) : _ish(0), _bnn(1) ? _bnn(1) : _ish(1) )
    , ish(_ish)
    , osh(bnn.apply(ish))
  {
    if (!size(ish))
      throw_error(modname, "Zero inout size.");
    if (!size(osh))
      throw_error(modname, "Zero result of binning"
                           " ("+toString(ish)+") by (" + toString(bnn) + ").");
    if ( std::all_of( whole(bnn), [](const ssize_t & bn){return std::abs(bn) == 1;}) )
      throw_bug(modname + ": tried to prepare OpenCL for trivial binning " + toString(_bnn) + ".");
    CL_isReady();
    for (CLenv & env : clenvs)
      try{ envs.emplace_back(env, ish, bnn); } catch(...) {}
    if (envs.empty())
      warn(modname, "Binning will be performed on CPUs.");
  }

  void operator() (const Map & imap, Map & omap) {
    if (imap.shape() != ish)
      throw_error(modname, "Missmatch of input shape ("+toString(imap.shape())+")"
                           " with expected ("+toString(ish)+").");
    if ( omap.shape() != osh)
      throw_error(modname, "Missmatch of output shape ("+toString(omap.shape())+")"
                           " with expected ("+toString(osh)+")."
                           " Arrays are assumed to be ready before getting here." );
    for (ForCLdev & env : envs)
      if (env.apply(imap,omap))
        return;
    // Do on CPU
    for (ssize_t y=0 ; y<osh(0) ; ++y) {
      const int bby = y < osh(0) ? bnn(0) : ish(0) % bnn(0);
      for (ssize_t x=0 ; x<osh(1) ; ++x) {
        const ssize_t bbx = x < osh(1) ? bnn(1) : ish(1) % bnn(1);
        float & val = omap(y,x);
        val = 0;
        for (ssize_t cy = 0 ; cy < bby ; cy++) {
          const ssize_t yy = y*bnn(0) + cy;
          for (ssize_t cx = 0 ; cx < bbx ; cx++)
            val += imap(yy, x*bnn(1) + cx);
        }
        val /= bbx*bby;
      }
    }
  }

};

const string Binn2D::modname = "BinnOCL";
cl_program Binn2D::clProgram = 0;



template<int Dim>
const string Binn<Dim>::modname = toString(Dim)+"D-binning";

template<int Dim>
Binn<Dim>::Binn(const string & str)
  : Binn( [](const string & str){
      if (str.empty())
        return Binn();
      const deque<string> subs = split(str, ",");
      int sbnn;
      Binn toRet;
      if ( 1 == subs.size() &&  1 != sscanf( str.c_str(), "%i", &sbnn ) )
        throw_error(modname, "Could not parse string \""+str+"\" as integer number.");
      if ( 1 != subs.size()  &&  Dim != subs.size() )
        throw_error(modname, "Could not parse string \""+str+"\". Must contain single or " + toString(Dim)
                             + " comma-delimited integers, while " + toString(subs.size()) + " found.");
      for (uint curD=0; curD<Dim; ++curD) {
        if ( Dim == subs.size() ) {
          const string & curStr = subs.at(curD);
          if (curStr.empty())
            sbnn=1;
          else if ( 1 != sscanf( curStr.c_str(), "%i", &sbnn ) )
            throw_error(modname, "Could not parse string \""+curStr+"\" as integer number"
                                 " for dimension " + toString(curD) + ".");
        }
        // Dim-1-curD here is to reflect the fact that natural x,y,z... in matrix notaition has inverted order
        toRet(Dim-1-curD) = sbnn ;
      }
      return toRet;
    }(str) )
{}

template<int Dim>
Binn<Dim>& Binn<Dim>::operator=(const Binn & other) {
  if (guts)
    specialize();
  blitz::TinyVector<ssize_t,Dim>::operator=(other) ;
  guts = other.guts;
  return *this;
}

template<int Dim>
Binn<Dim> Binn<Dim>::flipped() const {
  Binn<Dim> toRet;
  for (int dim=0; dim<Dim; dim++)
    toRet(dim) = abs((*this)(dim));
  return toRet;
}

template<int Dim>
Shape<Dim> Binn<Dim>::apply(const Shape<Dim> & ish) const {
  Shape<Dim> osh;
  for (uint curD=0; curD<Dim; ++curD)
    osh(curD) = binnOne( ish(curD), (*this)(curD) );
  return osh;
}

template<int Dim>
const ArrayF<Dim> Binn<Dim>::apply(const ArrayF<Dim> & iarr) const {
  ArrayF<Dim> toRet;
  apply(iarr,toRet);
  return toRet;
}

template<> void Binn<1>::subapply( const ArrayF<1> & iarr, ArrayF<1> & oarr) const {
  throw_bug("1D Binning is not implemented yet.");
  //oarr.resize(bnn.apply(iarr.shape()));
  // TODO
}

template<> void Binn<2>::specialize(const Shape<2> ish) const {
  Binn2D * proc = (Binn2D*) guts;
  if (proc) {
    if ( proc->ish == ish && proc->bnn != (*this) )
      return;
    else {
      delete proc;
      guts = 0;
      proc = 0;
    }
  }
  if ( flipOnly() || ! size(ish) )
    return;
  guts = new Binn2D(ish, *this);
}

template<int Dim> void Binn<Dim>::specialize(const Shape<Dim> ish) const {
  warn(modname, "Binning specialization is implemented for 2D only.");
  if (guts) { // Should never happen!
    delete (Binn2D*)guts;
    guts = 0;
  }
}

template<> void Binn<2>::subapply( const ArrayF<2> & iarr, ArrayF<2> & oarr) const {
  Binn2D * proc = (Binn2D*) guts;
  if ( proc && proc->ish == iarr.shape() && proc->bnn == *this )
    proc->operator()(iarr, oarr);
  else
    Binn2D(iarr.shape(), *this)(iarr, oarr);
}

template<> void Binn<3>::subapply( const ArrayF<3> & iarr, ArrayF<3> & oarr) const {

  const Shape<3> ish = iarr.shape();
  const Shape<3> osh = oarr.shape();
  const Binn<3> abnn(flipped());

  #ifdef OPENCL_FOUND

  try {

    CLmem clinarr(blitz2cl(iarr, CL_MEM_READ_ONLY));
    CLmem cloutarr(clAllocArray<float>(oarr.size(), CL_MEM_WRITE_ONLY));
    CLkernel kernelBinn3(Binn2D::binnProgram(), "binn3");

    kernelBinn3.setArg(0, clinarr());
    kernelBinn3.setArg(1, cloutarr());
    kernelBinn3.setArg(2, (cl_int) abnn(2));
    kernelBinn3.setArg(3, (cl_int) abnn(1));
    kernelBinn3.setArg(4, (cl_int) abnn(0));
    kernelBinn3.setArg(5, (cl_int) ish(2));
    kernelBinn3.setArg(6, (cl_int) ish(1));
    kernelBinn3.setArg(7, (cl_int) ish(0));

    kernelBinn3.exec(osh);
    cl2blitz(cloutarr(), oarr);

  }  catch (...) { // probably full volume was too big for the gpu

    warn("Binning", "Trying second method.");
    const Shape<2> sish(copyMost<2>(ish));
    const Binn<2> sbnn(copyMost<2>(abnn));
    const ssize_t zbnn = abnn(0);
    const Shape<2> sosh(copyMost<2>(osh));
    const size_t sosz(size(osh));

    Map tmpslice(sosh);
    CLmem cltmpslice(clAllocArray<float>(sosz));
    Map outslice(sosh);
    CLmem cloutslice(clAllocArray<float>(sosz));
    CLmem clinslice;
    CLkernel kernelBinn2(Binn2D::binnProgram(), "binn2");

    if (sbnn) {
      clinslice(clAllocArray<float>(size(sish), CL_MEM_READ_ONLY));
      kernelBinn2.setArg(0, clinslice());
      kernelBinn2.setArg(1, cltmpslice());
      kernelBinn2.setArg(2, (cl_int) sbnn(1));
      kernelBinn2.setArg(3, (cl_int) sbnn(0));
      kernelBinn2.setArg(4, (cl_int) sish(1));
      kernelBinn2.setArg(5, (cl_int) sish(0));
    }

    CLkernel kernelAddTo(Binn2D::binnProgram(), "addToSecond");
    kernelAddTo.setArg(0, cltmpslice());
    kernelAddTo.setArg(1, cloutslice());

    CLkernel kernelMulti(Binn2D::binnProgram(), "multiplyArray");
    kernelMulti.setArg(0, cloutslice());
    kernelMulti.setArg(1, (cl_float) zbnn);

    for (int z = 0  ;  z < osh(0)  ;  z++ ) {
      fillClArray(cloutslice(), sosz, 0);
      int cz;
      for (cz=0 ; cz<zbnn && z*zbnn+cz < ish(0) ; cz++) {
        Map inslice(iarr(z*zbnn+cz, all, all));
        if (sbnn) {
          blitz2cl(inslice, clinslice());
          kernelBinn2.exec(sosh);
        } else {
          blitz2cl(inslice, cltmpslice());
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

#else // OPENCL_FOUND

  throw_bug("Binning is implemented in OpenCL only.");

  const size_t bsz = bnn.x * bnn.y * bnn.z;
  for (ArrIndex zcur = 0 ; zcur < osh(0) ; zcur++ )
    for (ArrIndex ycur = 0 ; ycur < osh(1) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < osh(2) ; xcur++ )
        // BUG in blitz? But the following fails.
        //outarr(zcur,ycur,xcur) = mean(
        //  inarr( bnn.z  ?  blitz::Range(zcur*bnn.z, zcur*bnn.z+bnn.z-1)  :  all
        //       , bnn.y  ?  blitz::Range(ycur*bnn.y, ycur*bnn.y+bnn.y-1)  :  all
        //       , bnn.x  ?  blitz::Range(xcur*bnn.x, xcur*bnn.x+bnn.x-1)  :  all ));
      {
        float sum=0;
        for (int zbcur = 0 ; zbcur < bnn.z ; zbcur++ )
          for (int ybcur = 0 ; ybcur < bnn.y ; ybcur++ )
            for (int xbcur = 0 ; xbcur < bnn.x ; xbcur++ )
              sum += inarr( zcur*bnn.z+zbcur, ycur*bnn.y+ybcur, xcur*bnn.x+xbcur );
        outarr(zcur,ycur,xcur) = sum / bsz;
      }

#endif // OPENCL_FOUND

}


template<int Dim>
void Binn<Dim>::apply(const ArrayF<Dim> & iarr, ArrayF<Dim> & outarr) const {
  const Shape<Dim> ish = iarr.shape();
  const Shape<Dim> osh = apply(ish);
  ArrayF<Dim> fiarr(iarr);
  for (uint curD=0; curD<Dim; ++curD) // flipping
    if ((*this)(curD) < 0)
      fiarr.reverseSelf(curD);
  Binn<Dim> fbnn(flipped());
  if (flipOnly())
    outarr.reference(fiarr);
  else {
    outarr.resize(osh);
    subapply(fiarr, outarr);
  }
}

template class Binn<1>;
template class Binn<2>;
template class Binn<3>;

const string BinnOptionDesc =
  "Same for all dimensions if only one provided."
  " Reverses dimension if negative."
  " Averages over the whole dimension if zero.";




Shape<2> rotate(const Shape<2> & sh, float angle) {
  if ( abs( remainder(angle, M_PI/2) ) < 1.0/max(sh(0),sh(1)) ) { // close to a 90-deg step
    if ( ! ( ((int) round(2*angle/M_PI)) % 2 ) )
      return sh;
    else
      return Shape<2>(sh(1),sh(0));
  }
  const float cosa = cos(-angle), sina = sin(-angle);
  const int
    rwidth = abs( sh(1)*cosa ) + abs( sh(0)*sina),
    rheight = abs( sh(1)*sina ) + abs( sh(0)*cosa);
  return Shape<2>(rheight, rwidth);
}


void rotate(const Map & inarr, Map & outarr, float angle, float bg) {

  const Shape<2> sh = inarr.shape();
  const Shape<2> osh = rotate(sh, angle);

  if ( abs( remainder(angle, M_PI/2) ) < 1.0/max(sh(0),sh(1)) ) { // close to a 90-deg step
    const int nof90 = round(2*angle/M_PI);
    if ( ! (nof90%4)  ) { // 360deg
      outarr.reference(inarr);
      return;
    }
    outarr.resize(osh);
    if ( ! (nof90%2) ) //180deg
      outarr=inarr.copy().reverse(blitz::firstDim).reverse(blitz::secondDim);
    else if (  ( nof90 > 0 && (nof90%3) ) || ( nof90 < 0 && ! (nof90%3) ) )  // 270deg
      outarr=inarr.copy().transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::secondDim);
    else // 90deg
      outarr=inarr.copy().transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::firstDim);
    return;
  }

  const float cosa = cos(-angle), sina = sin(-angle);
  outarr.resize(osh);

  if ( ! isnormal(bg) ) {
    bg=0;
    bg += mean( inarr( all, 0 ) );
    bg += mean( inarr( 0, all ) );
    bg += mean( inarr( all, sh(1)-1 ) );
    bg += mean( inarr( sh(0)-1, all ) );
    bg /= 4.0;
  }

  const float
    constinx = ( sh(1) + osh(0)*sina - osh(1)*cosa ) / 2.0,
    constiny = ( sh(0) - osh(1)*sina - osh(0)*cosa ) / 2.0;

  for ( ssize_t x=0 ; x < osh(1) ; x++) {
    for ( ssize_t y=0 ; y < osh(0) ; y++) {

      float xf = x*cosa - y*sina + constinx;
      float yf = x*sina + y*cosa + constiny;
      const ssize_t flx = floor(xf), fly = floor(yf);
      xf -= flx;
      yf -= fly;

      if ( flx < 1 || flx >= sh(1)-1 || fly < 1  || fly >= sh(0)-1 ) {
        outarr(y, x)=bg;
      } else {
        float v0 = inarr(fly,flx) + ( inarr(fly,flx+1) - inarr(fly,flx) ) * xf;
        float v1 = inarr(fly+1,flx) + ( inarr(fly+1,flx+1) - inarr(fly+1,flx) ) * yf;
        outarr(y, x) = v0 + (v1-v0) * yf;
      }

    }
  }

}

void
rotate(Map & io_arr, float angle, float bg) {
  Map outarr;
  rotate(io_arr, outarr, angle, bg);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr.copy();
}


