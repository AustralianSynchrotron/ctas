#include "matrix.world.h"
#include "parallel.world.h"

using namespace std;


const string Segment::modname = "Segment";

Segment::Segment(const string & str) {
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
  const deque<string> subs = split(str, ",");
  if ( Dim != subs.size() )
    throw_error(modname, "Could not parse string \""+str+"\". Must contain " + toString(Dim)
                         + " comma-delimited substrings, while " + toString(subs.size()) + " found.");
  for (uint curD=0; curD<Dim; ++curD)
    // Dim-1-curD here is to reflect the fact that natural x,y,z... in matrix notaition has inverted order
    this->operator()(Dim-1-curD) = Segment(subs[curD]);
}

template<int Dim>
Shape<Dim> Crop<Dim>::apply(const blitz::TinyVector<ssize_t,Dim> & ish) const {
  Shape<Dim> osh;
  for (uint curD=0; curD<Dim; ++curD)
    osh(curD) = (*this)(curD).size(ish(curD));
  return osh;
}

template<int Dim>
const blitz::Array<float,Dim> Crop<Dim>::apply(const blitz::Array<float,Dim> & iarr) const {
  if (!(*this))
    return iarr;
  const Shape<Dim> osh = this->apply(iarr.shape());
  blitz::TinyVector<size_t,Dim> ubound;
  blitz::TinyVector<size_t,Dim> lbound;
  for (uint curD=0; curD<Dim; ++curD) {
    ubound[curD] = (*this)[curD].begin();
    lbound[curD] = ubound[curD] + osh[curD];
  }
  return iarr(blitz::RectDomain<Dim>(ubound, lbound));
}

template<int Dim>
void Crop<Dim>::apply(const blitz::Array<float,Dim> & inarr, blitz::Array<float,Dim> & outarr) const {
  const blitz::Array<float,Dim> oarr = this->apply(inarr);
  outarr.resize(oarr.shape());
  outarr = oarr;
}


template class Crop<1>;
template class Crop<2>;
template class Crop<3>;


const string CropOptionDesc = "Each SEG describes the segment of the corresponding axis."
  " SEG format is [UINT][[:+-]UINT], where fist number is the starting point of the segment"
  " and second one can be (depending on the delimiter)"
  " ':' - end point, '+' - length of the segment or '-' - crop from the end."
;






#ifdef OPENCL_FOUND
cl_program binnProgram = 0;
#endif



const string Binn3OptionDesc =
  "3D Binning factor(s) X:Y:Z. If zero - averages over the whole dimension.";


string
type_desc (Binn3*) {
  return "UINT[:UINT:UINT]";
}

int
_conversion (Binn3* _val, const string & in) {

  int x=0, y=0, z=0;

  if ( in.find_first_of(",:") !=  string::npos ) {

    int scanres = sscanf( in.c_str(), "%i:%i:%i", &x, &y, &z);
    if (scanres != 3) // try , instead of :
      scanres = sscanf( in.c_str(), "%i,%i,%i", &x, &y, &z);
    if ( 3 != scanres || x<0 || y<0 || z<0)
      return -1;

  } else {
    int xy;
    if ( 1 != sscanf( in.c_str(), "%i", &xy ) || xy<0 )
      return -1;
    x=xy;
    y=xy;
    z=xy;

  }

  *_val = Binn3(x, y, z);
  return 1;

}


void
binn(const Volume & inarr, Volume & outarr, const Binn3 & ibnn) {

  if ( ibnn.x == 1 && ibnn.y == 1 && ibnn.z == 1) {
    outarr.reference(inarr);
    return;
  }
  Binn3 bnn( ibnn.x ? ibnn.x : inarr.shape()(2) ,
             ibnn.y ? ibnn.y : inarr.shape()(1) ,
             ibnn.z ? ibnn.z : inarr.shape()(0) );
  Shape<3> osh( inarr.shape()(0) / bnn.z
            , inarr.shape()(1) / bnn.y
            , inarr.shape()(2) / bnn.x);
  outarr.resize(osh);

#ifdef OPENCL_FOUND

  static const string oclsrc = {
    #include "binn.cl.includeme"
  };
  binnProgram = initProgram(oclsrc, binnProgram, "Binn on OCL");
  if (!binnProgram)
      throw 1;

  try {

    CLmem clinarr(blitz2cl(inarr, CL_MEM_READ_ONLY));
    CLmem cloutarr(clAllocArray<float>(outarr.size(), CL_MEM_WRITE_ONLY));
    CLkernel kernelBinn3(binnProgram, "binn3");

    kernelBinn3.setArg(0, clinarr());
    kernelBinn3.setArg(1, cloutarr());
    kernelBinn3.setArg(2, (cl_int) bnn.x);
    kernelBinn3.setArg(3, (cl_int) bnn.y);
    kernelBinn3.setArg(4, (cl_int) bnn.z);
    kernelBinn3.setArg(5, (cl_int) inarr.shape()(2));
    kernelBinn3.setArg(6, (cl_int) inarr.shape()(1));
    kernelBinn3.setArg(7, (cl_int) osh(2));
    kernelBinn3.setArg(8, (cl_int) osh(1));

    kernelBinn3.exec(osh);
    cl2blitz(cloutarr(), outarr);

  }  catch (...) { // full volume was too big for the gpu

    warn("Binning", "Trying another method.");
    Map inslice(inarr.shape()(1), inarr.shape()(0));
    CLmem clinslice(clAllocArray<float>(inslice.size(), CL_MEM_READ_ONLY));
    Map outslice(osh(1), osh(0));
    CLmem cloutslice(clAllocArray<float>(outslice.size()));
    Map tmpslice(osh(1), osh(0));
    CLmem cltmpslice(clAllocArray<float>(outslice.size()));

    CLkernel kernelBinn2(binnProgram, "binn2");
    kernelBinn2.setArg(0, clinslice());
    kernelBinn2.setArg(1, cltmpslice());
    kernelBinn2.setArg(2, (cl_int) bnn.x);
    kernelBinn2.setArg(3, (cl_int) bnn.y);
    kernelBinn2.setArg(4, (cl_int) inslice.shape()(1));
    kernelBinn2.setArg(5, (cl_int) outslice.shape()(1));

    CLkernel kernelAddTo(binnProgram, "addToSecond");
    kernelAddTo.setArg(0, cltmpslice());
    kernelAddTo.setArg(1, cloutslice());

    CLkernel kernelMulti(binnProgram, "multiplyArray");
    kernelMulti.setArg(0, cloutslice());
    kernelMulti.setArg(1, (cl_float) bnn.z);

    for (int z = 0  ;  z < osh(0)  ;  z++ ) {
      fillClArray(cloutslice(), outslice.size(), 0);
      for (int cz=0 ; cz<bnn.z ; cz++) {
        inslice = inarr(z*bnn.z+cz, all, all);
        blitz2cl(inslice, clinslice());
        kernelBinn2.exec(outslice.shape());
        kernelAddTo.exec(outslice.size());
      }
      kernelMulti.exec(outslice.shape());
      cl2blitz(cloutslice(), outslice);
      outarr(z, all, all) = outslice;
    }

  }

#else // OPENCL_FOUND

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

void
binn(Volume & io_arr, const Binn3 & bnn) {
  Volume outarr;
  binn(io_arr, outarr, bnn);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr;
}





const string BinnOptionDesc =
  "2D Binning factor(s) X:Y. If zero - averages over the whole dimension.";

string
type_desc (Binn*) {
  return "UINT[:UINT]";
}

int
_conversion (Binn* _val, const string & in) {

  int x, y;

  if ( in.find_first_of(",:") !=  string::npos ) {

    int scanres = sscanf( in.c_str(), "%i:%i", &x, &y);
    if (scanres != 2) // try , instead of :
      scanres = sscanf( in.c_str(), "%i,%i", &x, &y);
    if ( 2 != scanres || x<0 || y<0 )
      return -1;

  } else {

    int xy;
    if ( 1 != sscanf( in.c_str(), "%i", &xy ) || xy<0 )
      return -1;
    x=xy;
    y=xy;

  }

  *_val = Binn(x, y);
  return 1;

}







class _BinnProc {

private:
  const Binn bnn;
  const Shape<2> ish;
  const Shape<2> osh;
  CLmem clinarr;
  CLmem cloutarr;
  CLkernel kernelBinn;

  static const string modname;
  static cl_program binnProgram;

public:

  _BinnProc(const Shape<2> & _ish, const Binn & _bnn)
    : bnn( _bnn.x ? _bnn.x : _ish(1), _bnn.y ? _bnn.y : _ish(0) )
    , ish(_ish)
    , osh(binn(ish,bnn))
    , kernelBinn(0)
  {
    if ( bnn.x == 1 && bnn.y == 1 )
      return;
    if (!area(ish))
      throw_error(modname, "Zero input size.");
    if (!area(osh))
      throw_error(modname, "Zero result of binning"
                           " ("+toString(ish)+") by (" + string(bnn) + ").");

    static const string oclsrc = {
      #include "binn.cl.includeme"
    };
    binnProgram = initProgram( oclsrc, binnProgram, modname);
    if (!binnProgram)
      throw_error(modname, "Could not initiate binning program.");
    clinarr(clAllocArray<float>(area(ish), CL_MEM_READ_ONLY));
    cloutarr(clAllocArray<float>(area(osh), CL_MEM_WRITE_ONLY));
    kernelBinn(binnProgram, "binn2");
    if ( ! kernelBinn || ! clinarr() || ! cloutarr() )
      throw_error(modname, "Failed to prepare for operation.");
    kernelBinn.setArg(0, clinarr());
    kernelBinn.setArg(1, cloutarr());
    kernelBinn.setArg(2, (cl_int) bnn.x);
    kernelBinn.setArg(3, (cl_int) bnn.y);
    kernelBinn.setArg(4, (cl_int) ish(1));
    kernelBinn.setArg(5, (cl_int) osh(1));

  }


  void operator() (const Map & imap, Map & omap) {
    if ( bnn.x == 1 && bnn.y == 1 ) {
      if (!omap.size())
        omap.reference(imap);
      else if (!areSame(omap, imap)){
        omap.resize(ish);
        omap = imap;
      }
      return;
    }
    if (imap.shape() != ish)
      throw_error(modname, "Missmatch of input shape ("+toString(imap.shape())+")"
                           " with expected ("+toString(ish)+").");
    blitz2cl(imap, clinarr());
    kernelBinn.exec(osh);
    omap.resize(osh);
    cl2blitz(cloutarr(), omap);
  }

};

const string _BinnProc::modname = "BinnOCL";
cl_program _BinnProc::binnProgram = 0;


BinnProc::BinnProc(const Shape<2> & ish, const Binn & bnn)
  : guts(new _BinnProc(ish,bnn))
{}

BinnProc::~BinnProc() {
  if (guts)
    delete (_BinnProc*) guts;
}

void BinnProc::operator() (const Map & imap, Map & omap) {
  if (guts)
    ((_BinnProc*) guts)->operator()(imap, omap);
}











Shape<2> rotate(const Shape<2> & sh, float angle) {
  if ( abs( remainder(angle, M_PI/2) ) < 1.0/max(sh(0),sh(1)) ) // close to a 90-deg step
    if ( ! ( ((int) round(2*angle/M_PI)) % 2 ) )
      return sh;
    else
      return Shape<2>(sh(1),sh(0));
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

      const float xf = x*cosa - y*sina + constinx;
      const float yf = x*sina + y*cosa + constiny;
      const ssize_t flx = floor(xf), fly = floor(yf);

      if ( flx < 1 || flx >= sh(1)-1 || fly < 1  || fly >= sh(0)-1 ) {
        outarr(y, x)=bg;
      } else {
        float v0 = inarr(fly,flx) + ( inarr(fly,flx+1) - inarr(fly,flx) ) * (xf-flx);
        float v1 = inarr(fly+1,flx) + ( inarr(fly+1,flx+1) - inarr(fly+1,flx) ) * (yf-fly);
        outarr(y, x) = v0 + (v1-v0) * (yf-fly);
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

