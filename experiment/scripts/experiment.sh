#!/bin/sh

list=("1000000" "2000000" "3000000" "4000000" "5000000" "6000000" "7000000" "8000000" "9000000" 
"10000000" "20000000" "30000000" "40000000" "50000000" "60000000" "70000000" "80000000" "90000000" 
"100000000" "200000000" "300000000")

for item in "${list[@]}" ; do
  set -- $item
  echo "generate query $1"

  nohup /usr/bin/time -f "#bit, MY, $1, %e sec, %M KB" ./bit_rank_select.out -m 0 -n $1 -q 1000000 >> "./log/bit_rank_select_my.log"
  nohup /usr/bin/time -f "#bit, BV, $1, %e sec, %M KB" ./bit_rank_select.out -m 2 -n $1 -q 1000000 >> "./log/bit_rank_select_bv.log"

  nohup /usr/bin/time -f "#psum, MY, $1, %e sec, %M KB" ./prefix_sum.out -m 0 -n $1 -q 1000000 >> "./log/prefix_sum_my.log"

  nohup /usr/bin/time -f "#WT, MY, sigma = 7, $1, %e sec, %M KB" ./rank_select.out -m 0 -n $1 -q 1000000 -a 7 >> "./log/rank_select_my_7.log"
  nohup /usr/bin/time -f "#WT, MY, sigma = 63, $1, %e sec, %M KB" ./rank_select.out -m 0 -n $1 -q 1000000 -a 63 >> "./log/rank_select_my_63.log"
  nohup /usr/bin/time -f "#WT, MY, sigma = 254, $1, %e sec, %M KB" ./rank_select.out -m 0 -n $1 -q 1000000 -a 254 >> "./log/rank_select_my_254.log"

done

for item in "${list[@]}" ; do
  set -- $item
  echo "generate query $1"

  nohup /usr/bin/time -f "#bit, DYNAMIC, $1, %e sec, %M KB" ./bit_rank_select.out -m 1 -n $1 -q 1000000 >> "./log/bit_rank_select_dynamic.log"

  nohup /usr/bin/time -f "#psum, DYNAMIC, $1, %e sec, %M KB" ./prefix_sum.out -m 1 -n $1 -q 1000000 >> "./log/prefix_sum_dynamic.log"

  nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 7, $1, %e sec, %M KB" ./rank_select.out -m 1 -n $1 1000000 $1 -a 7 >> "./log/rank_select_dynamic_7.log"
  nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 63, $1, %e sec, %M KB" ./rank_select.out -m 1 -n $1 1000000 $1 -a 63 >> "./log/rank_select_dynamic_63.log"
  nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 254, $1, %e sec, %M KB" ./rank_select.out -m 1 -n $1 1000000 $1 -a 254 >> "./log/rank_select_dynamic_254.log"

done



echo "Finished."