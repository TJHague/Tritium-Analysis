#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 0 16`;
  do
    if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
    then
      analyzer -l -q 'ecc_center.C('$j',"'"$i"'",1,1,"center_cut")'
    fi
  done
done
