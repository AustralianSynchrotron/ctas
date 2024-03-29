__kernel void ff(
    constant const size_t maxelement,
    global float* fg,
    global float* bg1,
    global float* bg2,
    global float* df1,
    global float* df2,
    const float bgw,
    const float dfw,
    const float cutOff)
{

  size_t l = get_global_id(0);
  if (l>=maxelement)
    return;

  float fgg = fg[l];
  float bg;
  if (bg1 && bg2)
    bg = bg1[l] * bgw + bg2[l] * (1-bgw);
  else if (bg1)
    bg = bg1[l];
  else if (bg2)
    bg = bg2[l];
  else
    bg = 1.0f;

  float df;
  if (df1 && df2)
    df = df1[l] * dfw + df2[l] * (1-dfw);
  else if (df1)
    df = df1[l];
  else if (df2)
    df = df2[l];
  else
    df = 0.0f;

  if ( df >= bg )
    fg[l] = 1.0f;
  else if (df >= fgg )
    fg[l] = 0.0f;
  else
    fg[l] = (fgg - df) / (bg-df);

  if ( cutOff>0.0f && fg[l] > cutOff )
    fg[l]=cutOff;

}



__kernel void ffm(
    constant const size_t maxelement,
    global float * io,
    constant const float * bg,
    constant const float * df,
    constant const float * dg,
    constant const float * mask )
{

  const size_t idx = get_global_id(0);
  if (idx>=maxelement)
    return;

  float midx = mask ? mask[idx] : 1.0f;
  if (midx==0.0) {
    io[idx] = 0.0f;
    return;
  }

  float fgg = io[idx];
  if (df)
    fgg -= df[idx];
  if (bg) {
    float bgg = bg[idx];
    if (dg)
      bgg -= dg[idx];
    else if (df)
      bgg -= df[idx];
    if (bgg == 0.0)
      fgg = 0.0f;
    else
      fgg /= bgg;
  }
  io[idx] = fgg;

}


