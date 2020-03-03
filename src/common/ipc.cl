kernel void filters (
  global float*            absFilter,
  global float*            phsFilter,
  int                      xx,
  int                      yy,
  float                    alpha,
  float                    aConst )
{

  const int index = get_global_id(0);
  const int j = index / xx;
  const int i = index % xx;

  if (index) {
    float ei, ej;
    ei = i / (float) xx;
    ej = j / (float) yy;
    if (ei>0.5) ei = 1.0 - ei;
    if (ej>0.5) ej = 1.0 - ej;
    float calc = ei*ei + ej*ej;
    absFilter[index] = calc / (calc+alpha) ;
    phsFilter[index] = aConst / (calc+alpha) ;
  } else {
    absFilter[index] = 1.0 / (1+alpha) ;
    phsFilter[index] = 0.0 ;
  }

}


kernel void applyFilter (
  global float*            filter,
  global float*            mid )
{
  const int index = get_global_id(0);
  mid[index] *= filter[index];
}



kernel void reset (global float* mid) {
  int cidx = 2*get_global_id(0);
  mid[cidx] = 0;
  mid[cidx+1] = 0;
}


kernel void inout (
  int                      forward,
  global float*            io,
  global float*            mid,
  int                      x,
  int                      xx,
  int                      sx,
  int                      sy )
{
  const int index = get_global_id(0);
  const int j = index / x;
  const int i = index % x;
  const int jj = j + sx;
  const int ii = i + sy;
  const int idx = 2 * (jj + xx*ii) ;
  if (forward)
    mid[idx] = 1 - io[index];
  else
    io[index] = mid[idx] ;
}


