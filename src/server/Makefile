CC = gcc
RM = rm
LIBS = -lpthread -lmysqlclient
INC = -I ../../include
SRC = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRC))
TARGET = server


all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)
%o:%c
	$(CC) -c $^
clean:
	$(RM) -f $(OBJS) $(TARGET)

.PHONY:all clean
