#!/bin/bash

# This script will build and install simon

mkdir build 2> /dev/null
cd build
# cmake -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` ..
cmake -DCMAKE_INSTALL_PREFIX=~/ ..

make -j9
sudo make install -j9
sudo ldconfig
kbuildsycoca4

echo -e "**** Build completed ****\n\nThe executable file \"simon\" is now ready and has been installed.\n\nIssue \"simon\" to start it."
