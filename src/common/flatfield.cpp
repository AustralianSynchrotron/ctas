
#include "flatfield.h"




Shape shapeMe(std::vector<Shape> shs) {
  Shape firstSh(0,0);
  for (int idx=0 ; idx < shs.size() ; idx++) {
    if ( ! area(firstSh) )
      firstSh = shs[idx];
    if ( area(shs[idx]) && shs[idx] != firstSh )
      throw_error("FlatFieldProc", "Non matching init shapes.");
  }
  return firstSh;
}


#ifdef ONGPU

cl_program FlatFieldProc::ffProgram = 0;

FlatFieldProc::FlatFieldProc( const Map & _bg, const Map & _df
                            , const Map & _dg, const Map & _ms)
  : sh( shapeMe( {_bg.shape(), _df.shape(), _dg.shape(), _ms.shape()} ) )
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bg( _bg.size() ? blitz2cl(_bg, CL_MEM_READ_ONLY) : 0)
  , df( _df.size() ? blitz2cl(_df, CL_MEM_READ_ONLY) : 0)
  , dg( _dg.size() ? blitz2cl(_dg, CL_MEM_READ_ONLY) : 0)
  , ms( _ms.size() ? blitz2cl(_ms, CL_MEM_READ_ONLY) : 0)
{
  if (!area(sh))
    return;
  static const std::string oclsrc = {
    #include "ff.cl.includeme"
  };
  ffProgram = initProgram(oclsrc, ffProgram, "Flat field on OCL");
  kernel(ffProgram, "ffm");
  kernel.setArg(0, io());
  kernel.setArg(1, bg());
  kernel.setArg(2, df());
  kernel.setArg(3, dg());
  kernel.setArg(4, ms());
}


FlatFieldProc::FlatFieldProc(const FlatFieldProc & other)
  : sh(other.sh)
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bg(other.bg)
  , df(other.df)
  , dg(other.dg)
  , ms(other.ms)
{
  if (!area(sh))
    return;
  kernel(ffProgram, "ffm");
  kernel.setArg(0, io());
  kernel.setArg(1, bg());
  kernel.setArg(2, df());
  kernel.setArg(3, dg());
  kernel.setArg(4, ms());
}


bool FlatFieldProc::execute(const Map & _io) {
  if ( !io() || !kernel )
    return 0;
  if (_io.shape() != sh)
    throw_error("FlatCL", "Non matching input shape");
  return blitz2cl(_io, io()) && kernel.exec(_io.size()) == CL_SUCCESS;
}


#else // ONGPU


FlatFieldProc::FlatFieldProc( const Map & _bg, const Map & _df
                            , const Map & _dg, const Map & _ms)
  : sh( shapeMe( {_bg.shape(), _df.shape(), _dg.shape(), _ms.shape()} ) )
  , df(_df)
  , bga(_bg.shape())
{
  if (!area(sh))
    return;
  if (!_bg.size()) {
    bga = 1.0;
    return;
  }

  bga = _bg;
  if (_dg.size())
    bga -= _dg;
  else if (_df.size())
    bga -= _df;

  for (ArrIndex ycur = 0 ; ycur < sh(0) ; ycur++ )
    for (ArrIndex xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & val = bga(ycur,xcur);
        if (_ms.size() && _ms(ycur,xcur)==0.0)
          val = 0.0;
        else if (val!=0.0)
          val = 1.0/val;
      }
}


FlatFieldProc::FlatFieldProc(const FlatFieldProc & other)
  : sh(other.sh)
  , df(other.df)
  , bga(other.bga)
{
  if (!area(sh))
    return;
}


Map & FlatFieldProc::process(Map & _io) const {
  if (!area(sh))
    return _io;
  if (_io.shape() != sh)
    throw_error("FlatFieldProc", "Non matching input shape");
  if (df.size())
    _io -= df;
  if (bga.size())
    _io *= bga;
  return _io;
}




#endif // ONGPU
