

kernel void  binn2(
  read_only global float*  in,
  write_only global float* out,
  int                      bx,
  int                      by,
  int                      iszx,
  int                      oszx)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  float sum = 0;
  for (int cy = 0 ; cy < by ; cy++) {
    int offy = (y*by + cy)*iszx;
    for (int cx = 0 ; cx < bx ; cx++)
      sum += in[x*bx + cx + offy];
  }
  out[x + y*oszx] = sum/(bx*by);
}


kernel void  binn3(
  read_only global float*  in,
  write_only global float* out,
  int                      bx,
  int                      by,
  int                      bz,
  int                      iszx,
  int                      iszy,
  int                      oszx,
  int                      oszy)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);
  float sum = 0;
  for (int cz = 0 ; cz < bz ; cz++) {
    int offz = iszy * iszx * (z*bz+cz);
    for (int cy = 0 ; cy < by ; cy++) {
      int offy = offz + iszx * (y*by+cy);
      for (int cx = 0 ; cx < bx ; cx++)
        sum += in[x*bx + cx + offy];
    }
  }
  out[x + y*oszx + z*oszy*oszx] = sum / (bx*by*bz);
}



kernel void  addToSecond(
  read_only global float*          in,
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

