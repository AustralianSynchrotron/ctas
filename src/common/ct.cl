kernel void ct_sino (
  global float*            slice,
  read_only image2d_t      sinogram,
  int                      pixels,
  int                      thetas,
  float                    center,
  global const float2*     cossins,
  sampler_t                sampler )
{

  const int index = get_global_id(0);
  const int j = index / pixels;
  const int i = index % pixels;
  const int hp = pixels / 2 ;

  if (  (i-hp)*(i-hp) + (j-hp)*(j-hp) >= hp * hp - 1 ) {
    slice[index]=0;
  } else {
    float total = 0.0f;
    for (size_t proj = 0; proj < thetas; proj++)  {
      const float2 cossin = cossins[proj];
      float offsetI = center + (1-cossin.x-cossin.y) * hp
                             + cossin.x * j + cossin.y * i;
      total += read_imagef(sinogram, sampler, (float2)(offsetI, proj)).x ;
    }
    slice[index] = total;
  }

}

kernel void ct_line (
  global float*            slice,
  read_only image2d_t      sinoline,
  int                      pixels,
  float                    center,
  float2                   cossin,
  sampler_t                sampler )
{

  const int index = get_global_id(0);
  const int j = index / pixels;
  const int i = index % pixels;
  const int hp = pixels / 2 ;

  //const int hpmc = hp - abs( (int) center );
  if (  (i-hp)*(i-hp) + (j-hp)*(j-hp) < hp * hp - 1 ) {
    const float offsetI = center + (1-cossin.y-cossin.x) * hp
                          + cossin.y * j + cossin.x * i;
    slice[index] += read_imagef(sinoline, sampler, (float2)(offsetI, 0)).x ;
  }

}




kernel void fbp (
    read_only  global float*  sino,
    write_only global float*  slice,
    int                       pixels,
    int                       thetas,
    float                     center,
    read_only  global float2* cossins)
{
  const int i = get_global_id(0);
  const int j = get_global_id(1);
  const int idx = i + j*pixels;
  const int hp = pixels / 2 ;
  if (  (i-hp)*(i-hp) + (j-hp)*(j-hp) >= hp * hp - 1 ) {
    slice[idx]=0;
    return;
  }
  float total = 0.0f;
  for (size_t proj = 0; proj < thetas; proj++)  {
    const float2 cossin = cossins[proj];
    int offsetI = center + (1-cossin.x-cossin.y) * hp
                    + cossin.x * j + cossin.y * i;
    total += sino[ offsetI + proj * pixels ];
  }
  slice[idx] = total;
}



