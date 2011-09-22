#!/bin/bash
rm $2
for i in `ls -1 $1/*.wav`; do
  while true; do
    aplay -r 16000 -c 1 -f S16_LE $i 2> /dev/null
    echo "Prompt (empty to hear again):"
    read -e test
    if [ -z $test ]; then
      echo "Again..."
    else
      if [ "$test" == "-" ]; then
        break;
      fi
      sample=`echo $i | sed -e 's/.*\///g' -e 's/\.wav//g'`
      echo "$sample $test" >> $2
      break;
    fi
  done
done
