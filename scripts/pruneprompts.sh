#!/bin/bash

function usage ()
{
	echo "Usage: $0 dict prompts_in prompts_out"
	exit 1
}

if [ -z "$3" ]; then
	usage;
fi

if [ -e $3 ]; then
  rm -i $3 || exit;
fi

cat $2 | while read i; do
  all_defined=1;
  first=1;
  for j in $i; do
    if [ $first == 1 ]; then
      first=0;
    else
      if [ "`grep -c "^$j[[:space:]]" $1`" == 0 ]; then
        echo "Not in dict: $j";
	all_defined=0;
      fi
    fi
  done;
  if [ $all_defined = 1 ]; then
    echo $i >> $3;
  else
    echo "Skipping $i";
  fi
done;
