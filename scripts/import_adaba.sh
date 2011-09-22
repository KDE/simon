#!/bin/bash
dict=/mnt/data/LM/all_de_words.txt
#if [[ -e prompts_de ]]; then
  #rm -i prompts_de || exit
#fi
#if [[ -e prompts_at ]]; then
  #rm -i prompts_at || exit
#fi
#if [[ -e prompts_ch ]]; then
  #rm -i prompts_ch || exit
#fi

declare -A TRANSLATION

skip=1;
#find $1 -iname "*.ogg" > filelist;
for i in `cat filelist`; do
  destdir=`dirname $i|sed 's/.*sounds\///'`;
  destname=`basename $i|tr -d '.ogg'`;
  if [[ "$destdir" = "E/EI/EINGELEGT" ]]; then 
    skip=0;
  fi
  if [[ $skip == 1 ]]; then
    continue;
  fi

  #echo "Destination: "$destdir"/"$destname;

  safetext=`echo $destname | sed -e 's/__.*//g' -e 's/!//g' -e 's/\.//g' -e 's/,//g'`
  text=`echo $safetext | sed  -e 's/_/ /g' -e 's/OE/Ö/g' -e 's/AE/Ä/g' -e 's/UE/Ü/g'`

  if [ "${TRANSLATION["$safetext"]}" != "" ]; then
    text=${TRANSLATION["$safetext"]};
    echo "Using previously translated text: $text"
  else
    if [ `echo $text|grep -e "Ö" -e "Ä" -e "Ü" -e "SS" -c -` != '0' ]; then
      if [ `grep -ci "^$text\$" /mnt/data/LM/all_de_words.txt` == '0' ]; then
        echo "Not in dict: "$text;
        echo "Enter correct text for $destname: "
        read usertext;
        if [ "$usertext" != '' ]; then
          text=$usertext;
        else
          echo "Empty text entered, keeping $text";
	  echo "$text" >> $dict;
        fi
        TRANSLATION["$safetext"]="$text";
      fi
    fi
  fi
  prompt="$destdir/$destname $text";

  echo "Prompt: $prompt";
  if [ `echo $destname|grep -c "__DE_" -` != '0' ]; then
    echo $prompt >> prompts_de;
  else
    if [ `echo $destname|grep -c "__AT_" -` != '0' ]; then
      echo $prompt >> prompts_at;
    else
      echo $prompt >> prompts_ch;
    fi
  fi
  mkdir -p $destdir 2> /dev/null;
  sox $i -r 16000 $destdir/$destname".wav" gain -8

done
rm filelist
