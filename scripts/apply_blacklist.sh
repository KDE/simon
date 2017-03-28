#!/bin/bash
# $1 prompty in
# $2 blacklist
 cat $1 | while read i; do file=`echo $i|awk '{print $1}'`; if [ `grep -c $file $2` = 0 ]; then echo $i; fi done 

