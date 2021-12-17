

kernel void  formframe (
  read_only global float*  im0,
  read_only global float*  im1,
  write_only global float* out,
  read_only global float*  gaps0,
  read_only global float*  gaps1 )
{
  const int idx = get_global_id(0);
  if ( gaps0[idx] == 0.0 and gaps0[idx] == 0.0 )
    out[idx] = closegap(idx);
  else if ( gaps0[idx] == 0.0 )
    out[idx] = im1[idx];
  else if ( gaps1[idx] == 0.0 )
    out[idx] = im0[idx];
  else {
    out[idx] = ( im0[idx]*gaps0[idx] + im1[idx]*gaps1[idx] ) / ( gaps0[idx] + gaps1[idx] ) ;
  }
}

