#!/bin/sh

INSTALL_PATH="$HOME/bin"
OUTPUT_FILE="mes_sim.exe"

FILES="src/main.c src/view.c src/model.c src/controler.c src/glass_info.c"

C_LINKED_LIST="-I$HOME/aclib/include/c_linked_list/ $HOME/aclib/c_linked_list/c_linked_list.c"
C_STRING="-I$HOME/aclib/include/c_string/ $HOME/aclib/c_string/c_string.c"
C_QUEUE="-I$HOME/aclib/include/c_queue/ $HOME/aclib/c_queue/c_queue.c"
C_TRIE="-I$HOME/aclib/include/c_trie/ $HOME/aclib/c_trie/c_trie.c"
S7LIB="$HOME/s7lib/s7lib.c $HOME/s7lib/s7lib_parser.c -I$HOME/s7lib/include/"

LIBS="$C_STRING $C_LINKED_LIST $C_QUEUE $C_TRIE $S7LIB -L. -lsnap7"

clear && gcc -O2 -Wall $FILES -g -o ./bin/$OUTPUT_FILE $LIBS #&& ./bin/$OUTPUT_FILE


#install

mkdir -p -v $INSTALL_PATH/
cp -v ./bin/$OUTPUT_FILE $INSTALL_PATH/$OUTPUT_FILE
