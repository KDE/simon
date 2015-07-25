#!/bin/bash
# $1 ... prompts
cat ../../../prompts | while read i; do 
  #echo "<s> "$(echo $i | sed 's/[^ ]* //')" </s> ("$(echo $i|sed -e 's/.*\///g' -e 's/ .*//')")"; 
  echo "<s> "$(echo $i | sed 's/[^ ]* //')" </s> ("$(echo $i|sed -e 's/ .*//')")"; 
done

