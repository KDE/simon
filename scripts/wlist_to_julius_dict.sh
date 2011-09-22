#!/bin/bash

#input: List of words (one per line -> uniq_prompts.sh)
#input: Lexicon
#output: dict

cat $1 | awk '{print "^"$0"\t"}' > testwords
grep -f testwords  $2 | awk '{printf "2\t"; sub($1"\t", "", $0); print $0}' |sort|uniq > $3
rm testwords
