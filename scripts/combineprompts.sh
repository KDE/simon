#!/bin/bash
rm promptslist prompts_combined 2> /dev/null;
for i in $*; do
	find $i -iname "prompts" >> promptslist;
done

cat promptslist | while read i; do
	datadir="`dirname $i`/"
	cat $i|while read promptsLine; do
		echo "$datadir$promptsLine" | sed 's/^\.\///' >> prompts_combined
	done;
done
rm promptslist;
