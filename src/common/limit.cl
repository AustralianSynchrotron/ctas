__kernel void limit_array(__global float* arr,
                          const float minimum,
                          const float maximum)
{
  int l = get_global_id(0);
  float element = arr[l];
  if ( element < minimum )
    arr[l] = minimum;
  else if ( element > maximum )
    arr[l] = maximum;
  else
    arr[l] = ( element - minimum ) / ( maximum - minimum ) ;
}
