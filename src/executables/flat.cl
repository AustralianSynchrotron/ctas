kernel void flat (
  global float*            ioim,
  global float*            tim,
  int                      xx,
  int                      yy,
  int                      rr )
{

  const int index = get_global_id(0);
  if (isnan(ioim[index]))
    return;
  const int j = index / xx;
  const int i = index % xx;

  float sum=0;
  int tot=0;

  const int ym=max(0, j-rr);
  const int yM=min(yy, j+rr);
  const int xm=max(0, i-rr);
  const int xM=min(xx, i+rr);
  const int rr2 = rr*rr;

  for(int y = ym ; y < yM ; y++) {
    const int xst = y*xx;
    const int yj2 = (y-j)*(y-j);
    for(int x = xm ; x < xM ; x++) {
      const float val=ioim[x+xst];
      if ( !isnan(val) &&  yj2 + (x-i)*(x-i) <= rr2 ) {
        tot++;
        sum += val;
      }
    }
  }

  tim[index] = sum == 0.0 ? 1.0 : ioim[index] * tot / sum;

}


kernel void minThreshold (
  global float*            ioim,
  float                    threshold )
{
  const int index = get_global_id(0);
  if ( isnan(ioim[index]) || ioim[index] > threshold )
    return;
  ioim[index]=NAN;
}


kernel void maxThreshold (
  global float*            ioim,
  float                    threshold )
{
  const int index = get_global_id(0);
  if ( isnan(ioim[index]) || ioim[index] < threshold )
    return;
  ioim[index]=NAN;
}


kernel void averageArr (
  global float*            ioim,
  int                      size,
  float*                   avrg )
{
  float sum=0;
  int tot=0;
  for(size_t idx=0 ; idx < size ; idx++) {
    const float dt = *data++;
    if (!isnan(dt)) {
      tot++;
      sum += dt;
    }
  }
  *avrg = tot ? sum/tot : NAN;  
}


kernel void normdata (
  global float*            ioim,
  global float*            tim,
  int                      size,
  int                      doavrg, 
  float                    avrg )
{
  const int index = get_global_id(0);
  const int elnan = isnan(*(ioim+index));
  if (doavrg && ! elnan)
    *(tim+index) *= avrg;
  else if ( ! doavrg && elnan)
    *(tim+index) /= avrg;    
}
