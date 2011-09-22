#!/bin/bash
find . -iname "*.wav" -exec ls -lh {} \;|grep ' 44 '| awk '{print $9}' > broken_samples.txt

