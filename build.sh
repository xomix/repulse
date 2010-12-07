#!/bin/bash
cd soundtouch
./configure
make
cd ..
cd Release
make
cp -p repulse ..
cd ..
