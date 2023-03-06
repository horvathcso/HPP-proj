#!/bin/bash

echo "Starting timings:"

echo "4x4 table:"
./test 4 input_4_empty
res1=$(./test 3 input_3_sud_ord_2)
res2=$(./test 3 input_3_sud_ord_3)
res3=$(./test 3 input_3_sud_ord_4)
echo "Sudokus:"
echo "$res1+$res2+$res3" |bc

res4=$(./test 3 input_3_78_6)
res5=$(./test 3 input_3_79_6)
echo "Shuffled"
echo "$res4+$res5" |bc
