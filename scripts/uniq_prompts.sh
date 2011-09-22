#!/bin/bash
rm .tmp 2> /dev/null
for i in `cat $1 | sed -e 's/\([a-z]\|[A-Z]\|\.\|[0-9]\|\/\|_\)* //'`; do echo $i >> .tmp; done
cat .tmp | sort | uniq
rm .tmp
