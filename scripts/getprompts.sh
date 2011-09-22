#!/bin/bash
function usage ()
{
	echo "Usage: $0 [-u user|-i institution] [h|s|g]"
	exit 1
}

if [ -z "$3" ]; then
	usage;
fi

if [ `mount | grep -c server` -lt 1 ]; then
  echo "Simon server not mounted"
  exit 1;
fi

channel=0; #all channels / no preprocessing
case $3 in
h)
  micconfig=headsets;;
s)
  micconfig=hanging;
  channel=2;;
g)
  micconfig=goosenecks;
  channel=1;;
*)
  usage;;
esac

microphones=`cat /mnt/data/configs/$micconfig`
if [ -z "$microphones" ]; then
  usage;
fi


query="use ssc;
select s.Path, s.Prompt
from Sample s inner join User u on s.UserId = u.UserId 
  left outer join UserInInstitution uii on u.UserId = uii.UserId
    inner join SampleType st on s.TypeId = st.SampleTypeId 
      inner join Microphone m on m.MicrophoneId = s.MicrophoneId
  WHERE st.ExactlyRepeated=1 and s.MicrophoneId in ($microphones)";

if [ $1 == "-u" ]; then
  query="$query and u.UserId in ($2)";
else
  if [ $1 == "-i" ]; then
    query="$query and uii.InstitutionId in ($2)";
  else
    usage;
  fi
fi

query="$query;"

echo "Extracting prompts..."
echo  $query | mysql -u root --password=<password> -h <host> ssc | sed '1d' | sed -e 's/\\\\/\//g' -e 's/.*samples\///ig' -e 's/\.wav\t/ /i' -e 's/ß/SS/g' | awk '{ test=substr($0, length($1)+2, length($0)); print $1" "toupper(test) }' > prompts

echo "Created prompts."

mkdir data 2> /dev/null;

for wav in `cat prompts | awk '{print $1".wav"}'`; do
  path=`echo $wav | awk -F / '{print $1}'`;
  mkdir data/$path 2> /dev/null;
  echo "Copying /mnt/server/$wav to data/$path";
  cp /mnt/server/$wav data/$path/;
done;

if [ $3 == "h" ]; then
  echo Done;
  exit 0;
fi

echo "Splitting wave files...";

echo "   Gathering channel information..."
find_multi_channel_files.sh

echo "   Finding stereo files..."
find_stereo.sh

echo "   Splitting stereo samples..."
split_stereo.sh

echo "   Splitting prompts file..."
split_prompts.sh

mv mono_prompts.txt prompts

echo "   Removing other files and merging result..."
grep --invert-match "_$channel " stereo_prompts.txt | awk '{print "data/"$1".wav"}' | xargs rm
grep "_$channel " stereo_prompts.txt > prompts
rm multi_channel_wav_list.txt stereo_recordings.txt stereo_prompts.txt

echo "Done";
