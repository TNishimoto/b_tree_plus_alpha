#!/bin/sh

list=("1000000" "5000000" "10000000" "50000000" "100000000" "500000000" "1000000000")

for item in "${list[@]}" ; do
  set -- $item
  echo "generate query $1"

  nohup /usr/bin/time -f "#bit, BTreePlusAlpha, n = $1, %e sec, %M KB" ./bit_rank_select.out -x BTreePlusAlpha -n $1 -q 1000000 >> "./log/bit_rank_select_my.log"

  # nohup /usr/bin/time -f "#bit, bit_vector, n = $1, %e sec, %M KB" ./bit_rank_select.out -x bit_vector -n $1 -q 1000000 >> "./log/bit_rank_select_bv.log"
  # nohup /usr/bin/time -f "#bit, DYNAMIC, n = $1, %e sec, %M KB" ./bit_rank_select.out -x DYNAMIC -n $1 -q 1000000 >> "./log/bit_rank_select_dynamic.log"

  nohup /usr/bin/time -f "#psum, BTreePlusAlpha, n = $1, %e sec, %M KB" ./prefix_sum.out -x BTreePlusAlpha -n $1 -q 1000000 >> "./log/prefix_sum_my.log"
  # nohup /usr/bin/time -f "#psum, DYNAMIC, n = $1, %e sec, %M KB" ./prefix_sum.out -x DYNAMIC -n $1 -q 1000000 >> "./log/prefix_sum_dynamic.log"

  nohup /usr/bin/time -f "#WT, BTreePlusAlpha, sigma = 7, n = $1, %e sec, %M KB" ./rank_select.out -x BTreePlusAlpha -n $1 -q 1000000 -a 7 >> "./log/rank_select_my_7.log"
  nohup /usr/bin/time -f "#WT, BTreePlusAlpha, sigma = 63, n = $1, %e sec, %M KB" ./rank_select.out -x BTreePlusAlpha -n $1 -q 1000000 -a 63 >> "./log/rank_select_my_63.log"
  nohup /usr/bin/time -f "#WT, BTreePlusAlpha, sigma = 254, n = $1, %e sec, %M KB" ./rank_select.out -x BTreePlusAlpha -n $1 -q 1000000 -a 254 >> "./log/rank_select_my_254.log"


  # nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 7, n = $1, %e sec, %M KB" ./rank_select.out -x DYNAMIC -n $1 -q 1000000 -a 7 >> "./log/rank_select_dynamic_7.log"
  # nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 63, n = $1, %e sec, %M KB" ./rank_select.out -x DYNAMIC -n $1 -q 1000000 -a 63 >> "./log/rank_select_dynamic_63.log"
  # nohup /usr/bin/time -f "#WT, DYNAMIC, sigma = 254, n = $1, %e sec, %M KB" ./rank_select.out -x DYNAMIC -n $1 -q 1000000 -a 254 >> "./log/rank_select_dynamic_254.log"

  # nohup /usr/bin/time -f "#Perm, BTreePlusAlpha, n = $1, %e sec, %M KB" ./permutation.out -x BTreePlusAlpha -n $1 -q 1000000 >> "./log/perm_my.log"


done

echo "Finished."