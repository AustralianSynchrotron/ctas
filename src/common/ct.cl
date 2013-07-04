kernel void ct_sino (
  global float*            slice,
  read_only image2d_t      sinogram,
  int                      pixels,
  int                      thetas,
  float                    center,
  constant float2*         cossins,
  sampler_t                sampler )
{

  const int index = get_global_id(0);
  const int j = index / pixels;
  const int i = index % pixels;
  const int hp = pixels / 2 ;

  const int hpmc = hp - abs( (int) center ) - 2 ;
  if (  (i-hp)*(i-hp) + (j-hp)*(j-hp) >= hpmc * hpmc ) {
    slice[index]=0;
  } else {
    float total = 0.0f;
    for (size_t proj = 0; proj < thetas; proj++)  {
      const float2 cossin = cossins[proj];
      const float offsetI = center + (1-cossin.y-cossin.x) * hp
                            + cossin.y * j + cossin.x * i;
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

  const int hpmc = hp - abs( (int) center );
  if (  (i-hp)*(i-hp) + (j-hp)*(j-hp) < hpmc * hpmc  ) {
    const float offsetI = center + (1-cossin.y-cossin.x) * hp
                          + cossin.y * j + cossin.x * i;
    slice[index] += read_imagef(sinoline, sampler, (float2)(offsetI, 0)).x ;
  }

}

