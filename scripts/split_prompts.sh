#!/bin/bash
rm stereo_prompts.txt 2> /dev/null
rm old_stereo_prompts.txt 2> /dev/null
for i in `cat stereo_recordings.txt`;
do
  filename=`echo $i | sed -e 's/\.wav//g' -e 's/\.\///g' -e 's/data\///g' `
  #stereo_prompt=`grep $filename good_prompts`
  stereo_prompt=`grep $filename prompts`
  echo $stereo_prompt >> old_stereo_prompts.txt
  echo `echo $stereo_prompt | awk '{sub(" ", "_1 ", $0); print $0}'` >> stereo_prompts.txt
  echo `echo $stereo_prompt | awk '{sub(" ", "_2 ", $0); print $0}'` >> stereo_prompts.txt
done
#grep -Fxvf old_stereo_prompts.txt good_prompts > mono_prompts.txt
grep -Fxvf old_stereo_prompts.txt prompts > mono_prompts.txt
rm old_stereo_prompts.txt

