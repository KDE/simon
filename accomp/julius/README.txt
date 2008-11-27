At the moment (27. 11. 2008) the latest Julius release (4.1) has a couple of bugs that will prevent it from working correctly with simon.

Please download the source release of the julius 4.1 version and extract it in this directory.

This will create the subdirectory "julius-4.1".

Then apply the patch using the following command:

cd julius-4.1
patch -p0 < ../julius-4.1_simon.patch


You can then go on and install Julius normally using 
./configure && make && make install


This will generate the following files in /usr/local/lib:
libsent.so and libjulius.so

If you have stale versions of libsent.a and libjulius.a (notice the .a extension) e.g. from a previous Julius installation please remove them before proceeding.
