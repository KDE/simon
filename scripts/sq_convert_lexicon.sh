#! /bin/sh
if [ -z "$3" ]; then
	echo "Error!"
	echo "Correct Syntax: sq_convert_lexicon.sh WORDLIST MODEL OUTPUT"
	exit 0
fi

#TMPFiles: TMP_sed_lexicon, TMP_lexicon, TMP_sq_lexicon
unset LANG
unset LC_ALL

cat $1 | awk '{print toupper($0)}' | sed -f /mnt/data/sequitur/scripts/umlautersatz.sed > TMP_sed_lexicon && g2p.py --model $2 --apply TMP_sed_lexicon > TMP_sq_lexicon && sed -f /mnt/data/sequitur/scripts/umlautersatz_retour.sed TMP_sq_lexicon | sort | uniq > $3
rm TMP_*
