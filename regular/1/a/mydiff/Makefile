.PHONY: all clean

# also see: https://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Warning-Options.html
defs = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
default = -std=c99 -pedantic -g -Wall  $(defs)
verbose = -std=c99 -pedantic -g3 -Wall -Wextra -Wwrite-strings -Wconversion -Wformat=2 -Warray-bounds -Wstack-protector -Wshadow $(defs)
library = -lpthread -pthread -lrt


all: mydiff
mydiff: mydiff.o
	gcc -o mydiff mydiff.o $(library) $(verbose)

mydiff.o: mydiff.c
		gcc -c mydiff.c $(verbose)
clean:
	shred -uvz mydiff *.o





