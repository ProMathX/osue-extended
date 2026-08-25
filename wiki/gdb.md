## Caveat lector

* You should really compile your program with the `-g` (with debugging symbols) switch.
* You might want to disable optimizations using the `-O0` switch; otherwise, the debugged code might jump between lines due to optimizations performed by the compiler.

## General usage

`gdb` is best started on an existing binary:

<pre>
gdb ./myprog
</pre>

`gdb` then greets you with basic information about itself, as well as a command-line interface:

<pre>
GNU gdb (GDB) 7.6.2
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <[http://gnu.org/licenses/gpl.html](http://gnu.org/licenses/gpl.html)>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-unknown-linux-gnu".
For bug reporting instructions, please see:
<[http://www.gnu.org/software/gdb/bugs/](http://www.gnu.org/software/gdb/bugs/)>...
Reading symbols from /tmp/test...done.
(gdb)
</pre>

There are two ways to pass arguments to your program. You can either use the `--args` switch when starting `gdb`:

<pre>gdb --args ./myprog one two three
</pre>

or you can pass them as arguments to the `run` command:

<pre>(gdb) run one two three
</pre>

**Ctrl+C stops the running program, but not `gdb`. To stop `gdb`, use Ctrl+D.**

## Useful commands

*   `run` starts executing the chosen program.
*   `kill` stops the programm being debugged.
*   `quit` quits `gdb`.
*   `bt` shows the backtrace (which function called which function?).
*   `up` and `down` move up and down the backtrace.
    *   `up _number_` moves directly to the backtrace level with the given number.
*   `list` shows the relevant chunk of source code the first time it is executed. Every subsequent call shows the next chunk of source code.
    *   `list _number_` shows the chunk of source code around the given line number in the current source file.
    *   `list _filename_:_number_` shows the chunk of source code around the given line number in the given source file.
*   `print _variable_` outputs the current value of a variable (or expression).
    *   `print (void*)str` shows the address of the string, while `print (char*)str` shows the value of the string itself.
*   `cont` (_Continue_) continues up to the next breakpoint (or until the program terminates or segfaults).
*   `step` (_Step Into_) evaluates an expression; if that expression is a function call, descends into the function call.
*   `next` (_Step Over_) evaluates an expression; if that expression is a function call, it is performed, but the debugger only pauses on the next expression in the current function.
*   `finish` (_Step Out_) continues evaluating expressions until the execution returns to the caller of the current function.
*   `break` sets a breakpoint.
    *   `break _function_` sets a breakpoint at the beginning of a specific function (e.g. `break printf`).
    *   `break _number_` sets a breakpoint at the given line in the current source file. If the line cannot be paused upon (because it is empty, or a comment, or there is otherwise nothing to evaluate), the breakpoint is placed onto the next "pausable" line.
    *   `break _filename_:_number_` (e.g. `break common.c:28`) sets a breakpoint at the given line in the given source file. The above warning about unpauseable lines applies as well.

### <span class="mw-headline" id="Useful_commands_for_low-level_debugging">Useful commands for low-level debugging</span>

Note: you probably won't need this for OSUE.

*   `i reg` shows the current value of all registers.
    *   `i reg _register_` shows the current value of the given register (e.g. `i reg rax`).
*   `return` immediately returns from the current function. (Do not confuse it with `finish`!) You may specify a value to return as an argument to the `return` command.

## Example

The following program segfaults:

<pre>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line = NULL;
    while (fgets(line, 1024, stdin) != NULL)
    {
        fputs(line, stdout);
    }
    free(line);
    return 0;
}
</pre>

Let's plug it into `gdb` to find out why:

<pre>
$ **gdb ./test**
GNU gdb (GDB) 7.6.2
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <[http://gnu.org/licenses/gpl.html](http://gnu.org/licenses/gpl.html)>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-unknown-linux-gnu".
For bug reporting instructions, please see:
<[http://www.gnu.org/software/gdb/bugs/](http://www.gnu.org/software/gdb/bugs/)>...
Reading symbols from /tmp/test...done.
(gdb) **run**
Starting program: /tmp/./test 
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
warning: Could not load shared library symbols for linux-vdso.so.1.
Do you need "set solib-search-path" or "set sysroot"?
**this is a test line I'm typing to stdin**

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7a9dc09 in __GI__IO_getline_info () from /usr/lib/libc.so.6
(gdb) 
</pre>

It's always a good idea to start with a backtrace.

<pre>
(gdb) **bt**
#0  0x00007ffff7a9dc09 in __GI__IO_getline_info () from /usr/lib/libc.so.6
#1  0x00007ffff7a9cb4f in fgets () from /usr/lib/libc.so.6
#2  0x00000000004006bd in main () at test.c:7
(gdb) 
</pre>

We're stuck in some funny internal function of libc here, so let's move up to our code.

<pre>
(gdb) **up 2**
#2  0x00000000004006bd in main () at test.c:7
7		while (fgets(line, 1024, stdin) != NULL)
(gdb) 
</pre>

Let's see some more context.

<pre>
(gdb) **list**
2	#include <stdlib.h>
3	
4	int main(void)
5	{
6		char *line = NULL;
7		while (fgets(line, 1024, stdin) != NULL)
8		{
9			fputs(line, stdout);
10		}
11		free(line);
(gdb) 
</pre>

Okay, is `stdin` set correctly?

<pre>
(gdb) **print stdin**
$1 = -136477472
(gdb) 
</pre>

That was pretty useless; let's look at it as a pointer.

<pre>
(gdb) **print (void*)stdin**
$2 = (void *) 0xfffffffff7dd84e0
(gdb) 
</pre>

Yeah, that looks plausible. What about `line`?

<pre>
(gdb) **print line**
$3 = 0x0
(gdb)
</pre>

Right. `line` hasn't been initialized. Let's see what happens if we dereference it:

<pre>
(gdb) **print *(char*)line**
Cannot access memory at address 0x0
(gdb)
</pre>

(This is what `gdb` says if you try dereferencing an invalid pointer, i.e. attempt to produce a segfault.)

Well, we found our bug. Let's stop debugging.

<pre>
(gdb) **quit**
A debugging session is active.

        Inferior 1 [process 4908] will be killed.

Quit anyway? (y or n) **y**
$
</pre>