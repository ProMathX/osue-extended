AddressSanitizer was originally implemented in [Clang](http://clang.llvm.org/) (since Clang 3.1), then ported to [GCC](http://gcc.gnu.org/) (since GCC 4.8). It is useful for diagnosing segmentation faults, memory corruption bugs, double free()s, and other memory management troubles. Similar to [Valgrind](/index.php/Valgrind "Valgrind")'s _memcheck_, it is faster, more accurate, and its output is far more user-friendly, but it must be explicitly compiled into the program, which may modify its timing behavior significantly.

## Caveat lector

*   Make sure you compiled your code using the `-g` switch, which generates debugging symbols and makes the output of AddressSanitizer far easier to follow.
*   Since a majority of memory accesses is checked, AddressSanitizer will slow down any but the most trivial programs.
*   Make sure that your program cleans up after itself properly even if an important system call fails or a signal must be handled.

## General usage

To compile AddressSanitizer support into your program, add the compiler flag `-fsanitize=address` to your GCC or Clang flags.

AddressSanitizer is automatically active when you run your program and crashes it immediately after an invalid memory access is detected.

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

When executed, the program outputs nothing since no invalid memory accesses were performed.

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

**The output**:

```
=================================================================
<span style="color:red;font-weight:bold">==10007==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60200000eff5 at pc 0x0000004d8d55 bp 0x7ffd644d0cb0 sp 0x7ffd644d0ca8</span>
<span style="color:blue;font-weight:bold">WRITE of size 1 at 0x60200000eff5 thread T0</span>
    #0 0x4d8d54 in main /tmp/fencepost.c:10:11
    #1 0x7f3f0dc5f60f in __libc_start_main (/usr/lib/libc.so.6+0x2060f)
    #2 0x417348 in _start (/tmp/test+0x417348)

<span style="color:green;font-weight:bold">0x60200000eff5 is located 0 bytes to the right of 5-byte region [0x60200000eff0,0x60200000eff5)</span>
<span style="color:purple;font-weight:bold">allocated by thread T0 here:</span>
    #0 0x4ad2f8 in __interceptor_malloc (/tmp/test+0x4ad2f8)
    #1 0x4d8cea in main /tmp/fencepost.c:6:15
    #2 0x7f3f0dc5f60f in __libc_start_main (/usr/lib/libc.so.6+0x2060f)

SUMMARY: AddressSanitizer: heap-buffer-overflow /tmp/fencepost.c:10:11 in main
Shadow bytes around the buggy address:
  0x0c047fff9da0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9db0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9dc0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9dd0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9de0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
=>0x0c047fff9df0: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>[05]<span style="color:red;font-weight:bold">fa</span>
  0x0c047fff9e00: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9e10: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9e20: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9e30: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
  0x0c047fff9e40: <span style="color:red;font-weight:bold">fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa</span>
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       <span style="color:red;font-weight:bold">fa</span>
  Heap right redzone:      <span style="color:red;font-weight:bold">fb</span>
  Freed heap region:       <span style="color:purple;font-weight:bold">fd</span>
  Stack left redzone:      <span style="color:red;font-weight:bold">f1</span>
  Stack mid redzone:       <span style="color:red;font-weight:bold">f2</span>
  Stack right redzone:     <span style="color:red;font-weight:bold">f3</span>
  Stack partial redzone:   <span style="color:red;font-weight:bold">f4</span>
  Stack after return:      <span style="color:purple;font-weight:bold">f5</span>
  Stack use after scope:   <span style="color:purple;font-weight:bold">f8</span>
  Global redzone:          <span style="color:red;font-weight:bold">f9</span>
  Global init order:       <span style="color:teal;font-weight:bold">f6</span>
  Poisoned by user:        <span style="color:blue;font-weight:bold">f7</span>
  Container overflow:      <span style="color:blue;font-weight:bold">fc</span>
  Array cookie:            <span style="color:red;font-weight:bold">ac</span>
  Intra object redzone:    <span style="color:olive;font-weight:bold">bb</span>
  ASan internal:           <span style="color:olive;font-weight:bold">fe</span>
  Left alloca redzone:     <span style="color:blue;font-weight:bold">ca</span>
  Right alloca redzone:    <span style="color:blue;font-weight:bold">cb</span>
==10007==ABORTING
```

This tells us that on line 10 of <tt>fencepost.c</tt>, an invalid write has been performed. It is right beyond the end of a five-byte block (_0 bytes to the right of 5-byte region_) which has been allocated by malloc on line 6 of the same file.

The memory map displayed in the middle shows us the memory layout around the addresses we are accessing. Each two hex digits correspond to a chunk of eight bytes. The value _00_ means that all those eight bytes can be accessed; _01_ to _07_ means that only the given number of bytes can be accessed; accessing bytes in a redzone is always invalid.

Since our five-byte chunk is surrounded by _fa_ bytes, which according to the legend means _Heap left redzone_, we can assume our invalid access is happening on the heap and not the stack.

Reports of invalid reads are almost identical.

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

**The output**:

```
=================================================================
<span style="color:red;font-weight:bold">==10601==ERROR: LeakSanitizer: detected memory leaks</span>

<span style="color:blue;font-weight:bold">Direct leak of 25 byte(s) in 5 object(s) allocated from:</span>
    #0 0x4ad2f8 in __interceptor_malloc (/tmp/test+0x4ad2f8)
    #1 0x4d8cfd in main /tmp/leak.c:9:16
    #2 0x7fc12d75f60f in __libc_start_main (/usr/lib/libc.so.6+0x2060f)

SUMMARY: AddressSanitizer: 25 byte(s) leaked in 5 allocation(s).
```

As we can see, 25 bytes have not been deallocated before the program exited; they were allocated in five (not necessarily same-sized) blocks on line 9 of <tt>leak.c</tt>.