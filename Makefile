CC=gcc
INCLUDES= 
LIBS= -lm
FILES= linklist.o types.o parser.o t00.o

CFLAGS += -g -Wall  -pedantic -std=c99

all: main

main: $(FILES)
	$(CC) $(CFLAGS) $(FILES) $(LIBS) -o  $@ 
	@echo "make $@ finished on `date`"

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o main

