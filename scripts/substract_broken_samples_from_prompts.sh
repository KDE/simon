#!/bin/bash
# Call with path to prompts
echo "#!/bin/bash" > temp.sh
echo "grep --invert-match \\" >> temp.sh
cat broken_samples.txt | sed -e 's/\.\///g' -e 's/\.wav//g' | awk '{print " -e "$0" \\"}'  >> temp.sh
echo ' $1' >> temp.sh
chmod +x temp.sh
if [ "`wc -l temp.sh`" != "3 temp.sh" ]; then
  ./temp.sh $1 > good_prompts
else
  cp $1 good_prompts
fi;

./remove_non_existant_samples.sh good_prompts
mv existing_prompts.txt good_prompts

rm "temp.sh"
