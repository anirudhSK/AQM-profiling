#! /bin/bash
algorithm=$1
gcc -fno-exceptions -g -c "$algorithm".c
objdump -w -j .text  -d "$algorithm".o > /tmp/"$algorithm".check
cat /tmp/"$algorithm".check | python path_explore.py 0
