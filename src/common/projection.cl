
kernel void  gauss (
  const int  Xs,
  const int  Ys,
  global float*  iom,
  global const float*  mask)
{
  const int idx = get_global_id(0);
  const int idy = get_global_id(1);
  const int idi = idx + idy * Xs;
  if (mask[idi] != 0.0 || sig <= 0.0)
    return;

  // find nearest unmasked
  const int mcrad = max(Xs, Ys) / 2;
  int crad=0; // negative crad indicates first nonmasked pixel
  while ( crad>=0 && crad < mcrad ) {
    crad++;
    const int cr2 = crad*crad;
    for (int dy = -crad ; dy <= crad ; dy++) {
      const int idyy = idy+dy;
      if (idyy<0 || idyy>=Ys)
        continue;
      const int dx = floor( sqrt((float)(cr2-dy*dy)) );
      const int cshft = idyy * Xs + idx;
      if  ( ( idx-dx >= 0   &&  mask[cshft-dx] != 0.0 )
         || ( idx+dx < Xs  &&  mask[cshft+dx] != 0.0 ) ) {
        crad *= -1; // indicator
        break;
      }
    }
  }
  if (crad>=0) // may happen only on full mask?
    return;
  const float sigma = -crad ; // final sigma, also negates previous indicator.
  const float sig22 = 2*sigma*sigma;
  const int mrad = floor(2.0*sigma);

  // filter
  float mass=0.0;
  float gsumd=0;
  for (int dy = -mrad ; dy <= mrad ; dy++) {
    const int idyy = idy+dy;
    if (idyy<0 || idyy>=Ys)
      continue;
    const int idiy = idyy * Xs;
    const int dy2 = dy*dy;
    const int xrad = floor(sqrt(2*sig22-dy2));
    for (int dx = -xrad ; dx <= xrad ; dx++) {
      int idxx = idx+dx;
      int idii = idiy + idxx;
      if (idxx<0 || idxx>=Xs || mask[idii]==0.0)
        continue;
      float wght = exp(-(dx*dx + dy2)/sig22);
      mass += wght;
      gsumd += wght*iom[idii];
    }
  }
  if (mass>0.0)
    iom[idi] = gsumd / mass;

}



