#!/bin/bash

# This script will build simon
# The executable will be called "simon" and located in this directory

cd src/
qmake && make

