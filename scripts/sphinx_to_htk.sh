#!/bin/bash
cat $1 | awk '{printf $1"\t["$1"]\t"; a=$0; sub($1, "", $0); print $0}'
