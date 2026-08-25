(Note that the sequencer and event counter synchronization primitives are themselves implemented using shared memory and semaphores.)

## General usage

The command line of <tt>strace</tt> has the following structure:

<pre>
strace STRACEOPTIONS PROGRAMNAME PROGRAMARG1 PROGRAMARG2...
</pre>

The simplest usage, which logs all system calls to <tt>stderr</tt>, is:

<pre>
strace ./forksort
</pre>

To log only specific system calls, use the <tt>-e</tt> option:

<pre>
strace -e open,close,read,write,fork,execve ./forksort
</pre>

To write the log into a file, use the <tt>-o</tt> option:

<pre>
strace -o f__ks__t.txt ./forksort
</pre>

To also log the system calls of programs that have forked off the one launched, use <tt>-f</tt>:

<pre>
strace -f ./forksort
</pre>

## Example output

The server program code is as follows:

<pre>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

enum
{
    LISTEN_BACKLOG = 5
};

static const char *progname = "dumbsock";
static const char * const msg = "LOLGTFO";
static int hear_sock = -1;
static int talk_sock = -1;

static void bail_out(const char *func)
{
    (void)fprintf(stderr, "%s: %s: %s\n", progname, func, strerror(errno));

    if (talk_sock != -1)
    {
        (void)close(talk_sock);
    }

    if (hear_sock != -1)
    {
        (void)close(hear_sock);
    }

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    struct sockaddr_in sin = {
        .sin_family = AF_INET,
        .sin_port = htons(1337),
        .sin_addr = { htonl(INADDR_ANY) }
    };

    if (argc > 0)
    {
        progname = argv[0];
    }

    hear_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hear_sock == -1)
    {
        bail_out("socket");
    }

    if (bind(hear_sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
        bail_out("bind");
    }

    if (listen(hear_sock, LISTEN_BACKLOG) == -1)
    {
        bail_out("listen");
    }

    if ((talk_sock = accept(hear_sock, NULL, NULL)) == -1)
    {
        bail_out("accept");
    }

    if (send(talk_sock, msg, strlen(msg), 0) != strlen(msg))
    {
        bail_out("send");
    }

    if (close(talk_sock) == -1)
    {
        bail_out("close");
    }
    talk_sock = -1;

    if (close(hear_sock) == -1)
    {
        bail_out("close");
    }
    hear_sock = -1;

    return 0;
}
</pre>

After a client connection, the output looks like this:

<pre>
execve("./dumbsock", ["./dumbsock"], [/* 54 vars */]) = 0
brk(0)                                  = 0x6ac000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=208990, ...}) = 0
mmap(NULL, 208990, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7faa0ec01000
close(3)                                = 0
open("/usr/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0000\30\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=2010709, ...}) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7faa0ec00000
mmap(NULL, 3828848, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7faa0e66d000
mprotect(0x7faa0e80a000, 2097152, PROT_NONE) = 0
mmap(0x7faa0ea0a000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19d000) = 0x7faa0ea0a000
mmap(0x7faa0ea10000, 15472, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7faa0ea10000
close(3)                                = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7faa0ebff000
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7faa0ebfe000
arch_prctl(ARCH_SET_FS, 0x7faa0ebff700) = 0
mprotect(0x7faa0ea0a000, 16384, PROT_READ) = 0
mprotect(0x7faa0ec35000, 4096, PROT_READ) = 0
munmap(0x7faa0ec01000, 208990)          = 0
socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) = 3
bind(3, {sa_family=AF_INET, sin_port=htons(1337), sin_addr=inet_addr("0.0.0.0")}, 16) = 0
listen(3, 5)                            = 0
accept(3, 0, NULL)                      = 4
sendto(4, "LOLGTFO", 7, 0, NULL, 0)     = 7
close(4)                                = 0
close(3)                                = 0
exit_group(0)                           = ?
</pre>

The interesting output only starts at the <tt>socket</tt> call, whereupon it documents which system calls were made by the program. As we can see, **<tt>send</tt> is only a wrapper function for the <tt>sendto</tt> system call**.

Of course, a filtered output might be a bit easier to follow; launching

<pre>
strace -o dumbsock.txt -e socket,bind,listen,accept,sendto,recvfrom,open,close ./dumbsock
</pre>

will produce the following output in <tt>dumbsock.txt</tt>:

<pre>
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
open("/usr/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) = 3
bind(3, {sa_family=AF_INET, sin_port=htons(1337), sin_addr=inet_addr("0.0.0.0")}, 16) = 0
listen(3, 5)                            = 0
accept(3, 0, NULL)                      = 4
sendto(4, "LOLGTFO", 7, 0, NULL, 0)     = 7
close(4)                                = 0
close(3)                                = 0
+++ exited with 0 +++
</pre>