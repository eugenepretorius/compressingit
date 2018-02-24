.PHONY : clean all

CC = gcc

CFLAGS  = 
CFLAGS += --std=gnu99
CFLAGS += -g
CFLAGS += -O1

LDFLAGS = -shared

SRCS  = ./src/byte_encoder.c
SRCS += ./src/test.c

OBJS = $(SRCS:.c=.o)


PROGRAM = bc_compress.dylib

all: $(PROGRAM)

./src/%.o: ./src/%.c
	$(CC) $(CFLAGS) -pg -c $^ -o $@

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(wildcard *.o)
	rm -f $(PROGRAM)
