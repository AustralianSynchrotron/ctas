
#include "flatfield.h"




Shape shapeMe(std::vector<Shape> shs) {
  Shape firstSh(0,0);
  for (int idx=0 ; idx < shs.size() ; idx++) {
    if ( ! area(firstSh) )
      firstSh = shs[idx];
    if ( area(shs[idx]) && shs[idx] != firstSh )
      throw_error("FlatCL", "Non matching init shapes.");
  }
  return firstSh;
}


cl_program FlatFieldProc::ffProgram() {
  const static char ffSource[] = {
    #include "ff.cl.includeme"
  };
  const static cl_program retme
      = initProgram( ffSource, sizeof(ffSource), "Flat field on OCL" );
  return retme;
}


FlatFieldProc::FlatFieldProc( const Map & _bg, const Map & _df
                            , const Map & _dg, const Map & _ms)
  : sh( shapeMe( {_bg.shape(), _df.shape(), _dg.shape(), _ms.shape()} ) )
  , kernel( area(sh) ? createKernel(ffProgram(), "ffm") : 0)
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bg( _bg.size() ? blitz2cl(_bg, CL_MEM_READ_ONLY) : 0)
  , df( _df.size() ? blitz2cl(_df, CL_MEM_READ_ONLY) : 0)
  , dg( _dg.size() ? blitz2cl(_dg, CL_MEM_READ_ONLY) : 0)
  , ms( _ms.size() ? blitz2cl(_ms, CL_MEM_READ_ONLY) : 0)
{
  if (!kernel)
    return;
  setArg(kernel, 0, io());
  setArg(kernel, 1, bg());
  setArg(kernel, 2, df());
  setArg(kernel, 3, dg());
  setArg(kernel, 4, ms());
}


FlatFieldProc::FlatFieldProc(const FlatFieldProc & other)
  : sh(other.sh)
  , kernel( area(sh) ? createKernel(ffProgram(), "ffm") : 0)
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bg(other.bg)
  , df(other.df)
  , dg(other.dg)
  , ms(other.ms)
{
  if (!kernel)
    return;
  setArg(kernel, 0, io());
  setArg(kernel, 1, bg());
  setArg(kernel, 2, df());
  setArg(kernel, 3, dg());
  setArg(kernel, 4, ms());
}


cl_mem FlatFieldProc::execute(const Map & _io) {
  if ( !io() || !kernel )
    return 0;
  if (_io.shape() != sh)
    throw_error("FlatCL", "Non matching input shape");
  return ( blitz2cl(_io, io()) && execKernel(kernel, _io.size()) == CL_SUCCESS )
      ? io() : 0;
}

