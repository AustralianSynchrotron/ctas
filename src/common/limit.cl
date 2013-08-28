__kernel void limit_array(__global float* arr,
                          const float minimum,
                          const float maximum)
{
  int l = get_global_id(0);
  float element = arr[l];
  if ( element <= minimum )
    arr[l] = 0;
  else if ( element >= maximum )
    arr[l] = 1.0;
  else
    arr[l] = ( element - minimum ) / ( maximum - minimum ) ;
}
