.PHONY : clean all

CC = gcc

CFLAGS  = 
CFLAGS += --std=gnu99
CFLAGS += -g
CFLAGS += -O1
CFLAGS += -DBE_COMPRESSION_TESTING
CFLAGS += -DDEBUG

LDFLAGS = 

SRCS  = ./src/byte_encoder.c
SRCS += ./src/byte_decoder.c
SRCS += ./src/test.c

OBJS = $(SRCS:.c=.o)


LIB = bc_compress.dylib
PROGRAM = bc_compress

all: $(PROGRAM) $(LIB)

./src/%.o: ./src/%.c
	$(CC) $(CFLAGS) -pg -c $^ -o $@

$(PROGRAM): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo 
	@echo To run python-tests type: nosetests -v ./pytest
	@echo 

$(LIB): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -shared
	@echo 
	@echo To run type: ./bc_compress
	@echo 

clean:
	rm -f $(OBJS) $(wildcard *.o)
	rm -f $(PROGRAM) $(LIB)
