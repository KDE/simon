#!/bin/bash
function usage ()
{
	echo "Usage: $0 [-u user|-i institution]"
	exit 1
}

if [ -z "$2" ]; then
	usage;
fi

mkdir {Headset,Haengemikrofon,Schwanenhals};
echo "============= Headset ===================="
cd Headset && getprompts.sh $1 $2 h

echo "============= Haengemikrofon ============="
cd ../Haengemikrofon && getprompts.sh $1 $2 s

echo "============= Schwanenhals ==============="
cd ../Schwanenhals && getprompts.sh $1 $2 g
