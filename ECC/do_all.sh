#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 0 16`;
  do
    if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
    then
      analyzer -l -q 'ecc.C('$j',"'"$i"'",1,1,"fix1")'
      analyzer -l -q 'ecc_up.C('$j',"'"$i"'",1,1,"up1")'
      analyzer -l -q 'ecc_down.C('$j',"'"$i"'",1,1,"down1")'
    fi
  done
done
