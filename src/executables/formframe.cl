

kernel void  formframe (
             global float*  out,
  read_only  global float*  im0,
  read_only  global float*  im1,
  read_only  global float*  gaps0,
  read_only  global float*  gaps1)
{
  const int idi = get_global_id(0);
  if ( gaps0[idi] == 0.0 && gaps1[idi] == 0.0 )
    out[idi] = 1.0; // 0.0 would be safe to see on the image?
  else if ( gaps0[idi] == 0.0 )
    out[idi] = im1[idi];
  else if ( gaps1[idi] == 0.0 )
    out[idi] = im0[idi];
  else
    out[idi] = ( im0[idi]*gaps0[idi] + im1[idi]*gaps1[idi] ) 
             / ( gaps0[idi] + gaps1[idi] ) ;
}


__constant const float sig = 0.44;
__constant const float gdiv = 2*sig*sig;

kernel void  eqnoise (
  read_only            int  Xs,
  read_only            int  Ys,
  read_only  global float*  iim,
             global float*  oim,
  read_only  global float*  gaps0,
  read_only  global float*  gaps1)
{  
  int idx = get_global_id(0);
  int idy = get_global_id(1);
  int idi = idx + idy * Xs;
  oim[idi]=iim[idi];
  float wght = gaps0[idi] + gaps1[idi];
  if (wght==0 || wght>=2.0) 
    return;

  // gaussian
  float mass=0.0;
  float gsum=0.0;
  bool isMax=true;
  bool isMin=true;
  for (int dy = -1 ; dy <= 1 ; dy++) {
    int idyy = idy+dy;
    if (idyy<0 || idyy>=Ys)
      continue;
    int idiy = idyy * Xs;
    for (int dx = -1 ; dx <= 1 ; dx++) {
      int idxx = idx+dx;
      if (idxx<0 || idxx>=Xs)
        continue;
      int idii = idiy + idxx;

      if ( gaps0[idii] + gaps1[idii] == 0.0 )
        continue;
      if (iim[idii]>iim[idi])
        isMax=false;
      if (iim[idii]<iim[idi])
        isMin=false;
      if (!isMax && !isMin)
        return;      
      float gwght = exp(-(dx*dx + dy*dy)/gdiv);
      mass += gwght;
      gsum += gwght * iim[idii];
    
    }
  }
  if (mass==0)
    return;
  oim[idi] = gsum/mass;

}

//// random
//float arr[8];
//for (int dy = -1 ; dy <= 1 ; dy++) {
//  int idyy = idy+dy;
//  int idiy = idyy * Xs;
//  for (int dx = -1 ; dx <= 1 ; dx++) {
//    int idxx = idx+dx; 
//    int idii = idiy + idxx;
//    if ( idii != idi  &&  gaps0[idii] + gaps1[idii] > 0.0 )
//      arr[dots++] = iim[idii];
//  }
//}
//if (!dots)
//  return;
//if (dots==1)
//  ov = arr[0];
//else {
//  float iv = iim[idi];
//  int prand = *(int*)(void*)(&iv);
//  ov = arr[prand % dots];
//}


//// mean
//float sumd=0;
//for (int dy = -1 ; dy <= 1 ; dy++) {
//  int idyy = idy+dy;
//  int idiy = idyy * Xs;
//  for (int dx = -1 ; dx <= 1 ; dx++) {
//    int idxx = idx+dx; 
//    int idii = idiy + idxx;
//    if ( idii != idi  &&  gaps0[idii] + gaps1[idii] > 0.0 ) {
//      sumd += iim[idii];
//      dots += 1;
//    }
//  }
//}
//if (!dots)
//  return;
//ov = sumd/dots;


//// median
//  float arr[8];
//  for (int dy = -1 ; dy <= 1 ; dy++) {
//    int idyy = idy+dy;
//    if (idyy<0 || idyy>=Ys)
//      continue;
//    int idiy = idyy * Xs;
//    for (int dx = -1 ; dx <= 1 ; dx++) {
//      int idxx = idx+dx;
//      if (idxx<0 || idxx>=Xs)
//       continue; 
//      int idii = idiy + idxx;
//      if ( idii != idi  &&  gaps0[idii] + gaps1[idii] > 0.0 )
//        arr[dots++] = iim[idii];
//    }
//  }
//  if (!dots)
//    return;
//
//  for (int i = 0; i < dots - 1; i++)
//    for (int j = 0; j < dots - i - 1; j++)
//      if (arr[j] > arr[j + 1]) {
//        float sw = arr[j];
//        arr[j] = arr[j+1];
//        arr[j+1] = sw; 
//      }
//  ov = arr[dots/2];



kernel void  gapfill (
  read_only            int  Xs,
  read_only            int  Ys,
  read_only            int  rr,
             global float*  io,
  read_only  global float*  gaps0,
  read_only  global float*  gaps1)
{
  const int idx = get_global_id(0);
  const int idy = get_global_id(1);
  const int idi = idx + idy * Xs;
  if ( gaps0[idi] != 0.0 || gaps1[idi] != 0.0 )
    return;

  float sum=0;
  int tot=0;

  const int ym=max(0, idy-rr);
  const int yM=min(Ys, idy+rr);
  const int xm=max(0, idx-rr);
  const int xM=min(Xs, idx+rr);
  const int rr2 = rr*rr;

  for(int y = ym ; y < yM ; y++) {
    const int idyy = y*Xs;
    const int yj2 = (y-idy)*(y-idy);
    for(int x = xm ; x < xM ; x++) {
      const int idj = idyy + x;
      if ( ( gaps0[idj] != 0.0 || gaps1[idj] != 0.0 )
           &&  yj2 + (x-idx)*(x-idx) <= rr2 ) {
        tot++;
        sum += io[idj];
      }
    }
  }

  if (tot)
    io[idi] = sum / tot;

}
