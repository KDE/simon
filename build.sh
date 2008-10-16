#!/bin/bash

# This script will build and install simon

cd build && cmake .. && make && su -c 'make install && ldconfig' && kbuildsycoca4 && echo -e "**** Build completed ****\n\nThe executable file \"simon\" is now ready and has been installed.\n\nIssue \"simon\" to start it."

