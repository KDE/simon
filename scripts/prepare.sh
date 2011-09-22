#!/bin/bash

# Call with path to prompts file
if [ -z "$1" ]; then
	echo "Usage: ./prepare <prompts file>"
	exit 1
fi

echo "Finding broken samples..."
./find_broken_samples.sh

echo "Removing broken samples... [1/2]"
./substract_broken_samples_from_prompts.sh $1

echo "Removing broken samples... [2/2]"
./remove_broken_samples.sh

echo "Gathering channel information..."
./find_multi_channel_files.sh

echo "Finding stereo files..."
./find_stereo.sh

echo "Splitting stereo samples..."
./split_stereo.sh

echo "Splitting prompts file..."
./split_prompts.sh


cat "mono_prompts.txt" "stereo_prompts.txt" > "all_prompts_final.txt"
