



kernel void fbp (
    global const float*       sino,
    global float*             slice,
    int                       pixels,
    int                       thetas,
    global const float2*      cossins,
    float                     center,
    float                     coeff)
{
  const int i = get_global_id(0);
  const int j = get_global_id(1);
  if (i>=pixels || j >= thetas)
    return;
  const int idx = i + j*pixels;
  const int hp = pixels / 2 ;
  const float acent = fabs(center);
  int ip2 = i-hp;
  ip2 *= ip2;
  float ijp2 = j-hp-center;
  ijp2 *= ijp2;
  ijp2 += ip2;
  if (  ( i <= acent  ||  i >= pixels-acent-1 )
     || ( j <= hp+center  &&  ijp2 >= (hp+center) * (hp+center) )
     || ( j >= hp+center  &&  ijp2 >= (hp-center) * (hp-center) ) )
  {
    slice[idx]=0;
    return;
  }
  float total = 0.0f;
  for (size_t proj = 0; proj < thetas; proj++)  {
    const float2 cossin = cossins[proj];
    int offsetI =   center + (1-cossin.x-cossin.y) * hp
                  + cossin.x * (j-center) + cossin.y * i;
    total += sino[ offsetI + proj * pixels ];
  }
  if (coeff != 0.0)
    total *= coeff;
    //total *= 2 / (pixelSize * 100);
  slice[idx] = total / thetas;
}



