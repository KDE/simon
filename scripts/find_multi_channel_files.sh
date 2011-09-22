#!/bin/bash
find . -iname "*.wav" -exec identify_mine.sh {} \; > multi_channel_wav_list.txt
