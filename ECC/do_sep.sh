#!/bin/sh

declare -a targs=("H1" "He3" "D2" "H3")

for i in "${targs[@]}"
do
  for j in `seq 7 16`;
  do
    if [ $j -lt 6 ];
    then
      analyzer -l -q 'ecc.C('$j',"'"$i"'",0,2,"out2_sep")'
    elif [ $j -gt 6 ] && [ $(( $j%2 )) -eq 1 ] && [ $j -ne 15 ] || [ $j -eq 16 ] 
    then
      analyzer -l -q 'ecc_separate.C('$j',"'"$i"'",0,2,"out2_sep",1)'
      analyzer -l -q 'ecc_separate.C('$j',"'"$i"'",0,2,"out2_sep",2)'
    elif [ $j -eq 15 ] 
    then
      analyzer -l -q 'ecc_separate.C('$j',"'"$i"'",0,2,"out2_sep",1)'
      analyzer -l -q 'ecc_separate.C('$j',"'"$i"'",0,2,"out2_sep",2)'
      analyzer -l -q 'ecc_separate.C('$j',"'"$i"'",0,2,"out2_sep",3)'
    fi
  done
done
