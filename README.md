# Welcome

This is a extended repository on [sueszli](https://github.com/sueszli/osue/) which is a [fork](https://github.com/osue-tuwien/exercises)


I created some templates, .clang-format and a generic Makefile on my gist:
1. https://gist.github.com/ProMathX/8671409e58877019d647575ca541271e

# ATTENTION
For the implentation NO AI WAS USED I REPEAT NO AI, nor in VSCODE.
Everything written in my own blood, tears and sweat, no goddamn clanker can replace us. Fuck clankers.

## The Programming setup:
    1. Neovim v0.12.3 with lsp's configured [dotfiles](https://github.com/ProMathX/dotfiles)
    2. Vscode (llvm-vs-code-extensions.vscode-clangd ms-vscode.cpptools cschlosser.doxdocgen)


## Practices on this codebase:
    1. Pull
    2. Make your own branch
    3. If your solution passes, then merge, only signed commits


## Sources:
    1. Private Obsidian notes


## Useful man pages

`man -k keyword`
`apropos keyword`

Here is a short list of the man pages I found most helpful in this course

_getopt_

- `man 3 getopt`
- `man 3 string`

_shared memory_

- `man 3 shm_open`
- `man 7 sem_overview`
- `man 7 shm_overview`

_fork, pipe, execle_

- `man 2 pipe` - also has fork
- `man 2 dup2`
- `man 2 wait`
- `man 3 system`

_socket_

- `man 2 select_tut` - also has sigaction
- `man 7 unix` - has overview
- `man 3 getaddrinfo` - fallback, if no ipv4 given
[src](https://github.com/sueszli/osue/tree/master)


paar wichtige manpages:

    man shm_overview -> ablauf was du wann machen musst für shm
    man sem_overview -> was du wann machen musst für semaphore
    man shm_open (ganz unten die examples sind basically ganzer ablauf den wir wissen müssenvorgegeben)
(Aus dem TU INF discord)


