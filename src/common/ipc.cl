
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

