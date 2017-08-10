#!/bin/bash
set -eu
cd build
make
mv efanna ../
cd ../
time ./efanna
#~ /usr/bin/time -v ./efanna
#~ valgrind --tool=callgrind ./efanna # use kcachegrind to read the callgrind.out.x file.
