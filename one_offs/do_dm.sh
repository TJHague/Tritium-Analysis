#!/bin/sh

for j in `seq 0 16`;
do
  if [ $j -lt 6 ] || [ $(( $j%2 )) -eq 1 ] || [ $j -eq 16 ];
  then
    analyzer -l -q 'em_dm_caps.C('$j',2,2,"em_dm")'
  fi
done
