#!/bin/bash
cd soundtouch
autoconf
aclocal
automake
./configure
make
cd ..
cd Release
make
cp -p repulse ..
cd ..
