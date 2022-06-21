
#include "flatfield.h"

const char ffSource[] = {
  #include "ff.cl.includeme"
};


const cl_program ffProgram =
  initProgram( ffSource, sizeof(ffSource), "Flat field on OCL" );



Shape shapeMe(std::vector<Shape> shs) {
  for (int idx=0 ; idx < shs.size() ; idx++)
    if ( area(shs[idx]) && shs[idx] != shs[0] )
      throw_error("FlatCL", "Non matching init shapes.");
  return shs.at(0);
}


FlatFieldProc::FlatFieldProc(const Map & _bg, const Map & _df,  const Map & _mask)
  : sh( shapeMe( {_bg.shape(), _df.shape(), _mask.shape()} ) )
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bgR( _bg.size() ? blitz2cl(_bg, CL_MEM_READ_ONLY) : 0)
  , bg(bgR)
  , dfR( _df.size() ? blitz2cl(_df, CL_MEM_READ_ONLY) : 0)
  , df(dfR)
  , maskR ( _mask.size() ? blitz2cl(_mask, CL_MEM_READ_ONLY) : 0)
  , mask(maskR)
  , kernel( area(sh) ? createKernel(ffProgram, "ffm") : 0)
{
  if (!kernel)
    return;
  setArg(kernel, 0, io());
  setArg(kernel, 1, bg());
  setArg(kernel, 2, df());
  setArg(kernel, 3, mask());
}


FlatFieldProc::FlatFieldProc(const FlatFieldProc & other)
  : sh(other.sh)
  , io( area(sh) ? clAllocArray<float>(area(sh)) : 0)
  , bg(other.bg)
  , df(other.df)
  , mask(other.mask)
  , kernel( area(sh) ? createKernel(ffProgram, "ffm") : 0)
{
  if (!kernel)
    return;
  setArg(kernel, 0, io());
  setArg(kernel, 1, bg());
  setArg(kernel, 2, df());
  setArg(kernel, 3, mask());
}


void FlatFieldProc::execute(const Map & _io) {
  if (!io())
    return;
  if (_io.shape() != sh)
    throw_error("FlatCL", "Non matching input shape");
  blitz2cl(_io, io());
  execKernel(kernel, _io.size());
}

