CFLAGS = -Wall -O -g
INCLUDE =  -I ../include

LIBS = -lpthread \
	   -lmysqlclient

CC = gcc

all:
	$(CC) server.c socket.c transit.c link.c version.c log.c self.c internet.c -o server $(INCLUDE) $(LIBS) $(CFLAGS)	
	$(CC) client.c -o client $(CFLAGS)
clean:
	rm server client 
