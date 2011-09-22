#!/bin/bash
cat multi_channel_wav_list.txt | grep 2$| sed 's/ 2$//g' > stereo_recordings.txt

