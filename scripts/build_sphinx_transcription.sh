#!/bin/bash
# $1 ... prompts
cat $1 | while read i; do 
  echo "<s> "$(echo $i | sed 's/[^ ]* //')" </s> ("$(echo $i|sed -e 's/ .*//')")"; 
done

