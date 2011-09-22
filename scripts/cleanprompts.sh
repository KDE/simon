#!/bin/bash

if [ -z "$3" ]; then
	echo "Usage: $0 blacklist prompts output"
	exit 1
fi
grep --invert-match -f $1  $2  > $3 
