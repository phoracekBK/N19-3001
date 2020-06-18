#!/usr/bin/sh


LIBS="-I$HOME/aclib/include/ -I$HOME/s7lib/include/ $HOME/s7lib/s7lib.c $HOME/s7lib/s7lib_parser.c $HOME/aclib/c_string/c_string.c -L$HOME/s7lib/lib/ -lsnap7"
FILES="src/main.c src/controler.c src/controler_parser.c src/csv_io.c"
DEST="bin/"
OUTPUT_FILENAME="csv_maker.exe"

clear && gcc -Wall -O2 $FILES $LIBS -o $DEST/$OUTPUT_FILENAME
