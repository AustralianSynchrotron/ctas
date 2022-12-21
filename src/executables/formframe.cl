

kernel void  formframe (
             global float*  out,
  global const float*  im0,
  global const float*  im1,
  global const float*  gaps0,
  global const float*  gaps1)
{
  const int idi = get_global_id(0);
  if ( gaps0[idi] == 0.0 && gaps1[idi] == 0.0 )
    out[idi] = 0.0; // 0.0 would be safe to see on the image?
  else if ( gaps0[idi] == 0.0 )
    out[idi] = im1[idi];
  else if ( gaps1[idi] == 0.0 )
    out[idi] = im0[idi];
  else
    out[idi] = ( im0[idi]*gaps0[idi] + im1[idi]*gaps1[idi] )
             / ( gaps0[idi] + gaps1[idi] ) ;
}


__constant const float sig = 0.45;
__constant const float gdiv = 2*sig*sig;

kernel void  eqnoise (
         const    int  Xs,
         const    int  Ys,
  global const float*  iim,
             global float*  oim,
  global const float*  gaps0,
  global const float*  gaps1)
{
  int idx = get_global_id(0);
  int idy = get_global_id(1);
  int idi = idx + idy * Xs;
  oim[idi]=iim[idi];
  float wght = gaps0[idi] + gaps1[idi];
  if (wght==0 || wght>=2.0)
    return;


  float mass=0.0;
  float gsumd=0;
  for (int dy = -1 ; dy <= 1 ; dy++) {
    int idyy = idy+dy;
    if (idyy<0 || idyy>=Ys)
      continue;
    int idiy = idyy * Xs;
    for (int dx = -1 ; dx <= 1 ; dx++) {
      int idxx = idx+dx;
      if (idyy<0 || idyy>=Ys)
        continue;
      int idii = idiy + idxx;
      float lwght = gaps0[idii] + gaps1[idii];
      if (lwght==0)
        continue;
      //float gwght = exp(-(dx*dx + dy*dy)/gdiv);
      mass += 1.0;
      gsumd += iim[idii];
    }
  }
  if (mass<=1)
    return;
  float gmean = gsumd/mass;

  gsumd=0.0;
  mass=0.0;
  for (int dy = -1 ; dy <= 1 ; dy++) {
    int idyy = idy+dy;
    if (idyy<0 || idyy>=Ys)
      continue;
    int idiy = idyy * Xs;
    for (int dx = -1 ; dx <= 1 ; dx++) {
      int idxx = idx+dx;
      if (idyy<0 || idyy>=Ys)
        continue;
      int idii = idiy + idxx;
      float lwght = gaps0[idii] + gaps1[idii];
      if (lwght==0)
        continue;
      float gwght = exp(-(dx*dx + dy*dy)/gdiv);
      float dd = iim[idii] - gmean;
      mass += gwght;
      gsumd += gwght * dd*dd;
    }
  }
  gsumd /= mass;
  oim[idi] = gmean + sign(iim[idi]-gmean) * sqrt(gsumd/2);
  return;

}


kernel void  gapfill (
  const int  Xs,
  const int  Ys,
  global float*  oim)
{
  const int idx = get_global_id(0);
  const int idy = get_global_id(1);
  const int idi = idx + idy * Xs;
  if (oim[idi]!=0.0)
    return;

  float sumd=0;
  float mass=0;
  float wght;
  int idyy, idxx, idii;

  #define findGood(coord, shift) \
    idyy=idy; \
    idxx=idx; \
    do { \
      coord += shift; \
      idii = idyy*Xs+idxx; \
    } while ( coord >= 0  &&  idyy < Ys  &&  idxx < Xs && \
            oim[idii] == 0.0 ); \
    if (coord >= 0  &&  idyy < Ys  &&  idxx < Xs) { \
      wght = 1.0 / ( abs(idyy-idy) + abs(idxx-idx) ); \
      mass += wght; \
      sumd += wght * oim[idii]; \
    }

  findGood(idxx,  1);
  findGood(idxx, -1);
  findGood(idyy,  1);
  findGood(idyy, -1);
  #undef findGood

  if (mass!=0.0)
    oim[idi] = sumd/mass;

}


