#!/bin/bash

h1="$1"
c1="$2"
h2="$3"
c2="$4"



acof=$( echo "scale=8; ( $c1 - $c2 ) / ( $h1 - $h2 ) " | bc  )
bcof=$( echo "scale=8; $c1 - $h1 * ( $c1 - $c2 ) / ( $h1 - $h2 ) " | bc  )
echo $bcof:$acof
