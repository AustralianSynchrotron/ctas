
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
  const int oszx = bx ? iszx / bx : 1;
  const int oszy = by ? iszy / by : 1;
  const int bbx = x < oszx ? bx : iszx % bx;
  const int bby = y < oszy ? by : iszy % by;
  float sum = 0;
  for (int cy = 0 ; cy < bby ; cy++) {
    const int offy = (y*by + cy)*iszx;
    for (int cx = 0 ; cx < bbx ; cx++)
      sum += in[x*bx + cx + offy];
  }
  out[x + y*(oszx + iszx % bx)] = sum/(bbx*bby);
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
  const int oszx = bx ? iszx / bx : 1;
  const int oszy = by ? iszy / by : 1;
  const int oszz = bz ? iszz / bz : 1;
  const int bbx = x < oszx ? bx : iszx % bx;
  const int bby = y < oszy ? by : iszy % by;
  const int bbz = z < oszz ? bz : iszz % bz;
  float sum = 0;
  for (int cz = 0 ; cz < bbz ; cz++) {
    int offz = iszy * iszx * (z*bz+cz);
    for (int cy = 0 ; cy < bby ; cy++) {
      int offy = offz + iszx * (y*by+cy);
      for (int cx = 0 ; cx < bbx ; cx++)
        sum += in[x*bx + cx + offy];
    }
  }
  out[x + (oszx + iszx % bx)*(y + z*(oszy + iszy % by))] = sum / (bbx*bby*bbz);
}



kernel void  addToSecond(
  global const float*    in,
  global float*          inout)
{
  const int idx = get_global_id(0);
  inout[idx] += in[idx];
}


kernel void  multiplyArray(
  global float*      inout,
  float              coeff)
{
  inout[get_global_id(0)] *= coeff;
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

