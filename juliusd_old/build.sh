#!/bin/bash

# This script will build simon
# The executable will be called "juliusd" and located in this directory

cd build/ && cmake .. && make &&  mv src/juliusd .. && cd .. &&  lrelease src/src.pro && echo -e "**** Build completed ****\n\nThe executable file \"juliusd\" is now ready.\n\nIssue \"./juliusd\" to start it."

