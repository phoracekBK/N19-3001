#!/bin/sh

OUTDIR="bin"
NAME="PLCUT.exe"

PLC="src/plc.c"
UNIT="src/unit.c"
MAIN="src/main.c"
FILES="$PLC $UNIT $MAIN"

C_STRING="$HOME/aclib/c_string/c_string.c -I$HOME/aclib/include/c_string/"
SNAP7="-L./ -lsnap7"
LIBS="$C_STRING $SNAP7"
SETTINGS="-Wall -O2"

#build
gcc -o $OUTDIR/$NAME $SETTINGS $FILES $LIBS

#install
cp -v $OUTDIR/$NAME $HOME/bin/$NAME
