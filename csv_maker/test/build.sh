#!/bin/sh

LIBS="-I$HOME/aclib/include/c_string/ -I$HOME/s7lib/include/ $HOME/s7lib/s7lib.c $HOME/s7lib/s7lib_parser.c $HOME/aclib/c_string/c_string.c -L$HOME/s7lib/ -lsnap7"
FILES="main.c unit_test.c ../src/controler_parser.c "
DEST="."
OUTPUT_FILENAME="csv_maker_test.exe"

gcc -Wall -O2 $FILES $LIBS -o $DEST/$OUTPUT_FILENAME
