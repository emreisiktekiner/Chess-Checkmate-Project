CFLAGS += -g -Wall -ansi -O3
LDFLAGS += -lm

all: main test

main: main.c io.c moves.c
	gcc $(CFLAGS) main.c io.c moves.c -o chess
 
test: test.c
	gcc -Wall -o test test.c -I$(PWD) -L$(PWD) -lrefimpl

libre_link:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD

.PHONY: clean
clean:
	rm -fv main test *.o
	rm -fv *~ \#*\# *.swp
