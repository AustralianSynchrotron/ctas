#!/bin/bash

read h1 c1 h2 c2 ;

acof=$( echo "scale=8; ( $c1 - $c2 ) / ( $h1 - $h2 ) " | bc  )
bcof=$( echo "scale=8; $c1 - $h1 * ( $c1 - $c2 ) / ( $h1 - $h2 ) " | bc  )
echo $bcof:$acof
