

kernel void  formframe (
             global float*  out,
  read_only  global float*  im0,
  read_only  global float*  im1,
  read_only  global float*  gaps0,
  read_only  global float*  gaps1)
{
  const int idi = get_global_id(0);
  if ( gaps0[idi] == 0.0 and gaps0[idi] == 0.0 )
    out[idi] = 1.0; // 0.0 would be safe to see on the image?
  else if ( gaps0[idi] == 0.0 )
    out[idi] = im1[idi];
  else if ( gaps1[idi] == 0.0 )
    out[idi] = im0[idi];
  else
    out[idi] = ( im0[idi]*gaps0[idi] + im1[idi]*gaps1[idi] ) / ( gaps0[idi] + gaps1[idi] ) ;
}


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
  const int idi = idx + idY * Xs;
  if ( gaps0[idi] != 0.0 || gaps0[idi] != 0.0 )
    return;

  float sum=0;
  int tot=0;

  const int ym=max(0, idy-rr);
  const int yM=min(Ys, idY+rr);
  const int xm=max(0, idx-rr);
  const int xM=min(Xs, idx+rr);
  const int rr2 = rr*rr;

  for(int y = ym ; y < yM ; y++) {
    const int idyy = y*xS;
    const int yj2 = (y-idy)*(y-idy);
    for(int x = xm ; x < xM ; x++) {
      const int idj = idyy + x;
      if ( ( gaps0[idj] != 0.0 || gaps0[idj] != 0.0 )
           &&  yj2 + (x-idx)*(x-idx) <= rr2 ) {
        tot++;
        sum += io[idj];
      }
    }
  }

  if (tot)
    io[idi] = sum / tot;

}
