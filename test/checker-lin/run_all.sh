#!/bin/bash

first_test=0
last_test=9
script=run_test.sh

# Call init to set up testing environment
bash ./_test/"$script" init

for i in $(seq $first_test $last_test); do
    bash ./_test/"$script" $i
done | tee results.txt

cat results.txt | grep '\[.*\]$' | awk -F '[] /[]+' '
BEGIN {
    sum=0
}

{
	sum += $(NF-2);
}

END {
    printf "\n%66s  [%02d/95]\n", "Total:", sum;
}'

# Cleanup testing environment
bash ./_test/"$script" cleanup
rm -f results.txt
