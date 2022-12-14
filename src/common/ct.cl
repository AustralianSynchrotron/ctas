



kernel void fbp (
    global const float*       sino,
    global float*             slice,
    int                       pixels,
    int                       thetas,
    float                     center,
    global const float2*      cossins)
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



