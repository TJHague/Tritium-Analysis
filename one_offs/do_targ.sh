#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 0 16`;
  do
    if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
    then
      analyzer -l -q 'targ_ec_rate.C('$j',"'"$i"'",2,2)'
    fi
  done
done
