

//kernel void  binn2(
//  global const float*      in,
//  global float*            out,
//  int                      bx,
//  int                      by,
//  int                      iszx,
//  int                      oszx)
//{
//  const int x = get_global_id(0);
//  const int y = get_global_id(1);
//  float sum = 0;
//  for (int cy = 0 ; cy < by ; cy++) {
//    const int offy = (y*by + cy)*iszx;
//    for (int cx = 0 ; cx < bx ; cx++)
//      sum += in[x*bx + cx + offy];
//  }
//  out[x + y*oszx] = sum/(bx*by);
//}


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

