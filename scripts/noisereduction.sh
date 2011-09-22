#!/bin/bash

function usage ()
{
	echo "Usage: $0 datadir"
	exit 1
}


if [ -z "$1" ]; then
	usage;
fi

find $1 -iname "*.wav" | while read i; do
  echo $i;
  normalize $i 2> /dev/null;
  sox $i $i.wav noisered /mnt/data/configs/noise_normalized.profile;
  mv $i.wav $i;
done
