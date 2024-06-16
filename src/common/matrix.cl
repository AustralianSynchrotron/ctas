
kernel void  binn2(
  global const float*      in,
  global float*            out,
  int                      bx,
  int                      by,
  int                      iszx,
  int                      iszy)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  if ( x >= iszx || y >= iszy )
    return;
  const int oszx = bx ? ( iszx + bx - 1 ) / bx : 1;
  const int oszy = by ? ( iszy + by - 1 ) / by : 1;
  const int bbx = min(iszx, (x+1)*bx) - x*bx;
  const int bby = min(iszy, (y+1)*by) - y*by;
  float sum = 0;
  for (int cy = 0 ; cy < bby ; cy++) {
    const int offy = (y*by + cy)*iszx;
    for (int cx = 0 ; cx < bbx ; cx++)
      sum += in[x*bx + cx + offy];
  }
  out[x + y*oszx] = sum/(bbx*bby);
}



kernel void  binn3(
  global const float*  in,
  global float*        out,
  int                  bx,
  int                  by,
  int                  bz,
  int                  iszx,
  int                  iszy,
  int                  iszz)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);
  if ( x >= iszx || y >= iszy || z >= iszz )
    return;
  const int oszx = bx ? ( iszx + bx - 1 ) / bx : 1;
  const int oszy = by ? ( iszy + by - 1 ) / by : 1;
  const int oszz = bz ? ( iszz + bz - 1 ) / bz : 1;
  const int bbx = min(iszx, (x+1)*bx) - x*bx;
  const int bby = min(iszy, (y+1)*by) - y*by;
  const int bbz = min(iszz, (z+1)*bz) - z*bz;
  float sum = 0;
  for (int cz = 0 ; cz < bbz ; cz++) {
    int offz = iszy * iszx * (z*bz+cz);
    for (int cy = 0 ; cy < bby ; cy++) {
      int offyz = offz + iszx * (y*by+cy);
      for (int cx = 0 ; cx < bbx ; cx++)
        sum += in[x*bx + cx + offyz];
    }
  }
  out[x + oszx*(y + z*oszy)] = sum / (bbx*bby*bbz);
}



kernel void  addToSecond(
  global const float*    in,
  global float*          inout,
  int                    len  )
{
  const int idx = get_global_id(0);
  if (idx>=len)
    return;
  inout[idx] += in[idx];
}


kernel void  multiplyArray(
  global float*      inout,
  float              coeff,
  int                len )
{
  const int idx = get_global_id(0);
  if (idx>=len)
    return;
  inout[idx] *= coeff;
}



kernel void  rotate2(
  global const float*      in,
  global float*            out,
  float                    bg,
  int                      iszx,
  int                      iszy,
  int                      oszx,
  global const float*     xf,
  global const float*     yf,
  global const int*       flx,
  global const int*       fly)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  if ( x >= iszx || y >= iszy )
    return;
  const int oshft = x+y*oszx;
  const int vflx = flx[oshft];
  const int vfly = fly[oshft];
  if ( vflx < 1 || vflx >= iszx-1 || vfly < 1  || vfly >= iszy-1 )
    out[oshft] = bg;
  else {
    const int ishft = vflx + vfly*iszx;
    float v0 = in[ishft]      + ( in[ishft+1]      - in[ishft]      ) * xf[oshft];
    float v1 = in[ishft+iszx] + ( in[ishft+iszx+1] - in[ishft+iszx] ) * yf[oshft];
    out[oshft] = v0 + (v1-v0) * yf[oshft];
  }
}




bool addMe(float val, float lo, float hi) {
  if (lo == hi)
    return val == hi;
  if (lo<hi)
    return val >= lo  &&  val <= hi;
  else
    return val >= lo  ||  val <= hi;
}

kernel void limitedSum(
  global float *input,
  int len,
  int offset,
  float lo,
  float hi
)
{

  const uint idx = get_global_id(0);
  const int myIdx = idx*2;
  if (myIdx >= len-1)
    return;

  if ( offset == 1 ) {

    int items=0;
    if ( addMe(input[myIdx], lo, hi) )
      ++items;
    else
      input[myIdx] = 0;
    if ( addMe(input[myIdx+1], lo, hi) ) {
      ++items;
      input[myIdx] += input[myIdx+1];
    }
    if ( myIdx == len-3  &&  addMe(input[myIdx+2], lo, hi) )  { // last element
      ++items;
      input[myIdx] += input[myIdx+2];
    }
    input[myIdx+1] = items;

  } else {

    const int myOff = myIdx*offset;
    input[myOff]   += input[myOff+offset];
    input[myOff+1] += input[myOff+offset+1];
    if ( myIdx == len-3 ) {  // last element
      input[myOff]   += input[myOff+2*offset];
      input[myOff+1] += input[myOff+2*offset+1];
    }

  }

}



