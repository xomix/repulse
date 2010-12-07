#!/bin/bash
cd soundtouch
autoreconf
autoconf
automake
./configure
make
cd ..
cd Release
make clean
make
cp -p repulse ..
cd ..
