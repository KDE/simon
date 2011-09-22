#!/bin/sh
#mplayer -vo null -ao null -frames 0 -identify "$@" 2>/dev/null |
	#sed -ne '/^ID_/ {
	                  #s/[]()|&;<>`'"'"'\\!$" []/\\&/g;p
	                #}' | sed -e ':a;N;$!ba;s/\n/ /g' | awk '{print $2" "$7}' | cut -c 13-
echo "$@ "`soxi -c "$@"`
