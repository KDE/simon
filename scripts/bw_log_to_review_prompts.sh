#!/bin/bash
#$1 bw log
#$2 prompts
for i in `head $1 -n 1000|grep -B 3 ERROR:|grep utt|awk '{print $2}'`; do awk "NR==$i" $2; done

