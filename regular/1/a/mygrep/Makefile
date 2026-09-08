.PHONY: all clean

# also see: https://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Warning-Options.html
defs = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
default = -std=c99 -pedantic -g -Wall  $(defs)
verbose = -std=c99 -pedantic -g3 -Wall -Wextra -Wwrite-strings -Wconversion -Wformat=2 -Warray-bounds -Wstack-protector -Wshadow $(defs)
library = -lpthread -pthread -lrt


all: myexpand
myexpand: myexpand.o
	gcc -o myexpand myexpand.o $(library) $(verbose)

myexpand.o: myexpand.c
		gcc -c myexpand.c $(verbose)
clean:
	shred -uvz myexpand *.o





