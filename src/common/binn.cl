

kernel void  binn2(
  read_only image2d_t      in,
  image2d_t                out,
  int                      bx,
  int                      by )
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  float sum = 0;
  for (int cy = 0 ; cy < by ; cy++)
    for (int cx = 0 ; cx < bx ; cx++)
      sum += read_imagef(in, (int2)(x*bx + cx, y*by + cy)).s0;
  write_image(out, (int2)(x,y), (float4)(sum/(bx*by),0,0,0));
}


kernel void  binn3(
  read_only image3d_t      in,
  image3d_t                out,
  int                      bx,
  int                      by,
  int                      bz )
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);
  float sum = 0;
  for (int cz = 0 ; cz < bz ; cz++)
    for (int cy = 0 ; cy < by ; cy++)
      for (int cx = 0 ; cx < bx ; cx++)
        sum += read_imagef(in, (int3)(x*bx + cx, y*by + cy, z*bz + cz)).s0;
  write_image(out, (int3)(x,y,z), (float4)(sum/(bz*bx*by),0,0,0));
}


kernel void  addToSecond(
  read_only image2d_t      in,
  image2d_t                inout)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  write_image(inout, (int2)(x,y), read_image(inout, (int2)(x,y)) + read_image(in, (int2)(x,y) );
}


kernel void  multiplyImage(
  image2d_t      inout,
  float          coeff)
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  write_image(inout, (int2)(x,y), coeff * read_image(inout, (int2)(x,y)) );
}



kernel void  binn2b(
  global float*            in,
  global float*            out,
  int                      xx,
  int                      yy,
  int                      bx,
  int                      by )
{
  const int idx = get_global_id(0);
  const int i = bx * idx % xx;
  const int j = by * idx / xx;
  float sum = 0;
  for (int cy = 0 ; cy < by ; cy++)
    for (int cx = 0 ; cx < bx ; cx++)
      sum += in[ i+cx + (j+cy)*xx ];
  out[idx] = sum / (bx*by);
}


kernel void  binn3b(
  global float*            in,
  global float*            out,
  int                      xx,
  int                      yy,
  int                      zz,
  int                      bx,
  int                      by,
  int                      bz)
{
  const int idx = get_global_id(0);
  const int i = bx * idx % xx;
  const int j = by * idx / xx;
  float sum = 0;
  for (int cy = 0 ; cy < by ; cy++)
    for (int cx = 0 ; cx < bx ; cx++)
      sum += in[ i+cx + (j+cy)*xx ];
  out[idx] = sum / (bx*by);
}
