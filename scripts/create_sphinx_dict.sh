#!/bin/bash
#$1...HTK lexicon
#$2...sphinx prompts (transcriptions)
# stdout: SPHINX lexicon
index=1
oldword=" "
cat $1 | while read i; do
  #echo "======"
  #echo $i
  word=`echo $i | sed 's/ .*//'`
  #if [ `grep -Fc " $word " $2` = 0 ]; then
    #continue;
  #fi
  transcription=`echo $i | sed 's/.*\] //'`
  if [ "$oldword" = "$word" ]; then
    index=$[$index + 1]
    echo -e "$word($index)\t$transcription"
  else
    index=1
    oldword=$word
    echo -e "$word\t$transcription"
  fi
done
  
