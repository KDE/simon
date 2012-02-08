#!/bin/bash

# This script will build and install simon

mkdir build 2> /dev/null
cd build && cmake -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` .. && make && sudo make install && sudo ldconfig && kbuildsycoca4 && echo -e "**** Build completed ****\n\nThe executable file \"simon\" is now ready and has been installed.\n\nIssue \"simon\" to start it."

