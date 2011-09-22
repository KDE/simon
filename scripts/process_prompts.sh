#!/bin/bash
sed '1d' $1 > temp_out
sed -e 's/\\\\/\//g' -e 's/.*samples\///g' -e 's/\.wav\t/ /' temp_out > $1
rm temp_out
