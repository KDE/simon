#!/bin/bash
if [ "`wc -l broken_samples.txt`" != "0 broken_samples.txt" ]; then
  cat broken_samples.txt | xargs rm 
else
  echo "No broken samples"
fi
