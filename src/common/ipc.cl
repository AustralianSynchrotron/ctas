/*

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
    absFilter[index] = 0.0 ;
    phsFilter[index] = (alpha == 0.0) ? 0.0 : aConst / alpha;
  } 

}

kernel void applyFilter (
  global float*            filter,
  global float*            mid )
{
  const int index = get_global_id(0);
  mid[2*index] *= filter[index];
  mid[2*index+1] *= filter[index];
}

*/


kernel void applyPhsFilter (
  global float*            mid,
  int                      xx,
  int                      yy,
  float                    alpha,
  float                    aConst )
{
  const int index = get_global_id(0);
  const int j = index / xx;
  const int i = index % xx;

  float ei = i / (float) xx;
  float ej = j / (float) yy;
  if (ei>0.5) ei = 1.0 - ei;
  if (ej>0.5) ej = 1.0 - ej;
  const float filter = index ? aConst / (ei*ei + ej*ej + alpha) : ( alpha == 0.0 ? 0.0 : aConst / alpha );

  mid[2*index] *= filter;
  mid[2*index+1] *= filter;

}

kernel void applyAbsFilter (
  global float*            mid,
  int                      xx,
  int                      yy,
  float                    alpha )
{
  const int index = get_global_id(0);
  const int j = index / xx;
  const int i = index % xx;
  
  float ei = i / (float) xx;
  float ej = j / (float) yy;
  if (ei>0.5) ei = 1.0 - ei;
  if (ej>0.5) ej = 1.0 - ej;
  const float filter = index ? 1 / (1 + alpha / (ei*ei + ej*ej) ) : 0.0 ;

  mid[2*index] *= filter;
  mid[2*index+1] *= filter;
  
}

kernel void applyZAbsFilter (global float* mid) {
  mid[0] = 0;
  mid[1] = 0;  
}

kernel void applyZPhsFilter (global float* mid, float zConst) {
  mid[0] *= zConst;
  mid[1] *= zConst;
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


