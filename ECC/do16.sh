#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 16 16`;
  do
    if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
    then
#      analyzer -l -q 'ecc.C('$j',"'"$i"'",1.5,1.5,"sig1_5")'
#      analyzer -l -q 'ecc.C('$j',"'"$i"'",2,2,"sig2")'
      analyzer -l -q 'ecc.C('$j',"'"$i"'",0,2,"out2_ACCfinal")'
#      analyzer -l -q 'DM_ecc.C('$j',"'"$i"'",1.5,1.5,"DM1_5")'
#      analyzer -l -q 'DM_ecc.C('$j',"'"$i"'",2,2,"DM2")'
#      analyzer -l -q 'DM_ecc.C('$j',"'"$i"'",0,2,"DMout2")'
    fi
  done
done
