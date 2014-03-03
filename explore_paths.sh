#! /bin/bash
algorithm=$1
rm "$algorithm".o
rm "$algorithm".check
gcc -fno-exceptions -g -c "$algorithm".c
objdump -w -j .text  -d "$algorithm".o > "$algorithm".check
cat "$algorithm".check | python path_explore.py 0
