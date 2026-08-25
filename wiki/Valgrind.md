It is useful for diagnosing segmentation faults, memory corruption bugs, double free()s, and other memory management troubles.

## Caveat lector

*   Make sure you compiled your code using the <tt>-g</tt> switch, which generates debugging symbols and makes the output of Valgrind far easier to understand.
*   Valgrind sometimes has troubles detecting out-of-bounds access to variables on the stack.
*   Since a majority of memory accesses is checked, Valgrind will considerably slow down any but the most trivial programs.
*   Make sure that your program cleans up after itself properly even if an important system call fails or a signal must be handled.

## General usage

The command line of <tt>valgrind</tt> has the following structure:

<pre>
valgrind VALGRINDOPTIONS PROGRAMNAME PROGRAMARG1 PROGRAMARG2...
</pre>

The simplest usage is

<pre>
valgrind ./fencepost
</pre>

To also trace where still-allocated or leaked memory was originally allocated, use the <tt>--leak-check=full</tt> option:

<pre>
valgrind --leak-check=full ./leaker
</pre>

## Example outputs

### Healthy program

**The code:**

```
#include <stdlib.h>

int main(void)
{
	size_t i;
	char *test = malloc(5 * sizeof(*test));

	for (i = 0; i < 5; ++i)
	{
		test[i] = 'a' + i;
	}

	free(test);
	return 0;
}
```

**The output:**

```
==23049== Memcheck, a memory error detector
==23049== Copyright (C) 2002-2012, and GNU GPL'd, by Julian Seward et al.
==23049== Using Valgrind-3.8.1 and LibVEX; rerun with -h for copyright info
==23049== Command: ./fencepost
==23049== 
==23049== 
==23049== HEAP SUMMARY:
==23049==     in use at exit: 0 bytes in 0 blocks
==23049==   total heap usage: 1 allocs, 1 frees, 5 bytes allocated
==23049== 
==23049== All heap blocks were freed -- no leaks are possible
==23049== 
==23049== For counts of detected and suppressed errors, rerun with: -v
==23049== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 2 from 2)
```

(Don't worry about suppressed errors; they are mostly related to C library quirks or Valgrind's instrumentation trickery.)

### Out-of-bounds access

**The code:**

```
#include <stdlib.h>

int main(void)
{
	size_t i;
	char *test = malloc(5 * sizeof(*test));

	for (i = 0; i <= 5; ++i)
	{
		test[i] = 'a' + i;
	}

	free(test);
	return 0;
}
```

**The error:** Despite the array being of size 5 (indices 0 through 4), the loop also accesses the array at index 5 due to an invalid condition.

**The output** (filtered to contain relevant information only):

```
==23077== Command: ./fencepost
==23077== 
==23077== Invalid write of size 1
==23077==    at 0x40061C: main (fencepost.c:10)
==23077==  Address 0x51da045 is 0 bytes after a block of size 5 alloc'd
==23077==    at 0x4C2C04B: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==23077==    by 0x4005ED: main (fencepost.c:6)
==23077== 
==23077== 
==23077== HEAP SUMMARY:
==23077==     in use at exit: 0 bytes in 0 blocks
==23077==   total heap usage: 1 allocs, 1 frees, 5 bytes allocated
```

This tells us that on line 10 of <tt>fencepost.c</tt>, an invalid write has been performed. It is right beyond the end of a five-byte block (_0 bytes after a block of size 5_) which has been allocated by malloc on line 6 of the same file. Invalid reads look pretty much the same.

### Memory leak

**The code:**

```
#include <stdlib.h>

int main(void)
{
	size_t i;

	for (i = 0; i < 5; ++i)
	{
		char *test = malloc(5 * sizeof(*test));
	}

	return 0;
}
```

**The error:** Memory is allocated in a loop, and the returned pointer is "forgotten" with each iteration, leading to a memory leak (the last pointer to a block is lost before this block can be deallocated).

**The output** (filtered):

```
==23179== Command: ./fencepost
==23179== 
==23179== 
==23179== HEAP SUMMARY:
==23179==     in use at exit: 25 bytes in 5 blocks
==23179==   total heap usage: 5 allocs, 0 frees, 25 bytes allocated
==23179== 
==23179== 25 bytes in 5 blocks are definitely lost in loss record 1 of 1
==23179==    at 0x4C2C04B: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==23179==    by 0x4005C3: main (fencepost.c:9)
==23179== 
==23179== LEAK SUMMARY:
==23179==    definitely lost: 25 bytes in 5 blocks
==23179==    indirectly lost: 0 bytes in 0 blocks
==23179==      possibly lost: 0 bytes in 0 blocks
==23179==    still reachable: 0 bytes in 0 blocks
==23179==         suppressed: 0 bytes in 0 blocks
==23179==
```

As we can see, 25 bytes have not been deallocated before the program exited; they were allocated in five (not necessarily same-sized) blocks on line 9 of <tt>fencepost.c</tt>.