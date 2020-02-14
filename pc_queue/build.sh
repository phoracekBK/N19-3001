#!/bin/sh


FILES="src/main.c src/plc.c src/model.c src/controler.c src/glass_info.c"

C_LINKED_LIST="-I$HOME/aclib/include/c_linked_list/ $HOME/aclib/c_linked_list/c_linked_list.c"
C_STRING="-I$HOME/aclib/include/c_string/ $HOME/aclib/c_string/c_string.c"
C_QUEUE="-I$HOME/aclib/include/c_queue/ $HOME/aclib/c_queue/c_queue.c"

LIBS="$C_STRING $C_LINKED_LIST $C_QUEUE -L. -lsnap7"

gcc -O2 -Wall $FILES -g -o ./bin/pc_queue.exe $LIBS && ./bin/pc_queue.exe -d

