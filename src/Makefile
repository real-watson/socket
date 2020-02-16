CFLAGS = -Wall -O -g
INCLUDE =  -I ../include

LIBS = -lpthread \
	   -lmysqlclient

CC = gcc

target:
	$(CC) server.c socket.c transit.c link.c version.c -o server $(INCLUDE) $(LIBS) $(CFLAGS)	
	$(CC) client.c -o client $(CFLAGS)
clean:
	rm pull
