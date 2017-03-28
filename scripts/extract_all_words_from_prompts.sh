#!/bin/bash
#LANG="" cat $1 | sed -e 's/^.*\/\?[0-9][0-9]\?\/[0-9][0-9]\?[0-9]\?[0-9]\?[0-9]\?\(_[0-9]\)\? //g' | sed  -e 's/ /\n/g'|  sort | uniq
cat $1| sed 's/^[^ ]\+//g'| sed 's/ /\n/g' |LANG="" sort|uniq
