#!/bin/bash
rm existing_prompts.txt

#for i in `cat $1`; do
cat $1 | while read i; do
  file="`echo $i | awk '{print $1".wav"}'`";
  if [ -s $file ]; then
    echo $i >> existing_prompts.txt
  else
    echo "File doesn't exist $file"
  fi
done
