CC = gcc
RM = rm
INC = -I ../../include
SRC = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRC))
TARGET = client


all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^
%o:%c
	$(CC) -c $^
clean:
	$(RM) -f $(OBJS) $(TARGET)

.PHONY:all clean
