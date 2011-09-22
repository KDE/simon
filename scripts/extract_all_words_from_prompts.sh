#!/bin/bash
#LANG="" cat $1 | sed -e 's/^.*\/\?[0-9][0-9]\?\/[0-9][0-9]\?[0-9]\?[0-9]\?[0-9]\?\(_[0-9]\)\? //g' | sed  -e 's/ /\n/g'|  sort | uniq
LANG="" cat $1 | sed -e 's/^.* //g' | sed  -e 's/ /\n/g'|  sort | uniq
