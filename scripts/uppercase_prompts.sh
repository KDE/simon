#!/bin/bash
cat $1 | awk '{ test=substr($0, length($1)+2, length($0)); print $1" "toupper(test) }' > $1.t
mv $1.t $1

