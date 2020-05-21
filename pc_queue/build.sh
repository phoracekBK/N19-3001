#!/bin/sh


FILES="src/main.c src/model.c src/controler.c src/glass_info.c"

C_LINKED_LIST="-I$HOME/aclib/include/c_linked_list/ $HOME/aclib/c_linked_list/c_linked_list.c"
C_STRING="-I$HOME/aclib/include/c_string/ $HOME/aclib/c_string/c_string.c"
C_QUEUE="-I$HOME/aclib/include/c_queue/ $HOME/aclib/c_queue/c_queue.c"
S7LIB="$HOME/s7lib/s7lib.c $HOME/s7lib/s7lib_parser.c -I$HOME/s7lib/include/"

LIBS="$C_STRING $C_LINKED_LIST $C_QUEUE $S7LIB -L. -lsnap7"

gcc -Wall -O2 $FILES -o ./bin/pc_queue.exe $LIBS #&& ./bin/pc_queue.exe -d
