#!/bin/sh
#add arm arm-linux-gnueabi-gcc
#arm-linux-gnueabi-gcc -fPIC -c socket.c
#arm-linux-gnueabi-gcc -fPIC -shared -o libsocket.so socket.o

gcc -fPIC -c socket.c
gcc -fPIC -shared -o libsocket.so socket.o
cp libsocket.so /lib -fa
gcc main.c -o test -lsocket
echo "built done"
