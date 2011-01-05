#!/bin/bash

TMP_FILE=/tmp/code
sed 's/.*\s\s*lm\s\s*.*/__64BIT__/g' /proc/cpuinfo | grep __64BIT__&>/dev/null
[ $? == 0 ] && BIT64=1 || BIT64=0
[ $1 ] && MODE=Debug || MODE=Release
[ $BIT64 == 1 ] && echo "Compiling for 64 bits..." || echo "Compiling for 32 bits..."
echo "Compiling in $MODE mode..."

# Build SoundTouch
cd soundtouch
patch -l -N -p0 <soundtouch-pedantic.diff
if [ $BIT64 == 1 ]; then
    patch -l -N -p0 <soundtouch-x86_64-asm-broken.diff
    patch -l -N -p0 <soundtouch-remove-sse.diff
fi
autoreconf
autoconf
automake
./configure
make
cd ..

# Build repulse
cd $MODE
if [ $BIT64 == 1 ]; then
    for i in $(find -type f | grep ".*\.mk$"); do
	sed 's/.msse2//g' "$i" > "$TMP_FILE"
	cp "$TMP_FILE" "$i"
    done
fi
make clean
make

# Copy binary
cp -p repulse ..
cd ..
