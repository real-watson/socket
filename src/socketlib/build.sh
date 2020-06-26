#!/bin/sh

gcc -fPIC -c socket.c
gcc -fPIC -shared -o libsocket.so socket.o
cp libsocket.so /lib -fa
gcc main.c -o test -lsocket
echo "built done"
