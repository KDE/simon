#!/bin/bash

for i in `cat stereo_recordings.txt`;
do
  sox $i `echo $i | sed 's/\.wav$//g'`_1.wav remix 1;
  sox $i `echo $i | sed 's/\.wav$//g'`_2.wav remix 2;
  rm $i;
done

