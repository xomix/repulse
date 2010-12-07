#!/bin/bash
cd soundtouch
./configure
make
cd ..
cd Debug
make
cp -p repulse ..
cd ..
