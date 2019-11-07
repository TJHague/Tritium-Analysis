#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 0 16`;
  do
    if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
    then
      analyzer -l -q 'DM_ecc.C('$j',"'"$i"'",0,2,"DMfix2")'
      analyzer -l -q 'DM_ecc.C('$j',"'"$i"'",1,1,"DMsym")'
    fi
  done
done
