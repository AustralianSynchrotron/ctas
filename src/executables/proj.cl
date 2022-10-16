
kernel void  gauss (
  read_only            int  Xs,
  read_only            int  Ys,
             global float*  iom,
  read_only  global float*  mask,
  read_only          float  sig )
{
  const int idx = get_global_id(0);
  const int idy = get_global_id(1);
  const int idi = idx + idy * Xs;
  if (mask[idi] != 0.0 || sig <= 0.0)
    return;
  const int mrad = ceil(2*sig);
  const float sig22 = 2*sig*sig;

  float mass=0.0;
  float gsumd=0;
  for (int dy = -mrad ; dy <= mrad ; dy++) {
    int idyy = idy+dy;
    if (idyy<0 || idyy>=Ys)
      continue;
    int idiy = idyy * Xs;
    for (int dx = -mrad ; dx <= mrad ; dx++) {
      int idxx = idx+dx;
      int idii = idiy + idxx;
      if (idxx<0 || idxx>=Xs || mask[idii]==0.0)
        continue;
      int dr2 = dx*dx + dy*dy;
      if (dr2 > mrad*mrad)
        continue;
      float wght = exp(-dr2/sig22);
      mass += wght;
      gsumd += wght*iom[idii];
    }
  }
  if (mass>0.0)
    iom[idi] = gsumd / mass;

}



