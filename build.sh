#!/bin/bash

# This script will build simon
# The executable will be called "simon" and located in this directory

cd src/
qmake && lupdate src.pro && make && lrelease src.pro && mv *.qm .. && echo -e "**** Build completed ****\n\nThe executable file \"simon\" is now ready.\n\nIssue \"./simon\" to start it."

