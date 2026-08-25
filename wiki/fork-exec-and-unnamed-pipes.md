While the 3 mechanisms fork, exec and unnamed pipes are very different from each other they are often used together.

* fork: On Linux systems new processes are created during the _fork(2)_ call which duplicates the current process. The original process is called _parent process_ and the duplicate is called _child process_. The two processes do differentiate from each other in only one part: _fork()_ returns zero to the child process and returns the _process id_ of the duplicated process (child process) to the parent process. So, the two processes have different process ids (and different parents), but are related to each other.

From the programmer's point of view, the return value of _fork(2)_ is most interesting - depending on it, the programmer can choose how the two processes should behave after the _fork(2)_ call completed: otherwise the two processes would continue in parallel to execute exactly the same program path. It is possible (and should be done!) to collect the exit status of a terminated child with _wait(2)_ and _waitpid(2)_.

* exec: All _exec(3)_ functions replace the current process image with a new process image that needs to be loaded from the file system. Together with _fork(2)_ it allows for starting new programs: e.g. First the shell forks and the _child process_ overwrites itself with the process image of the program the user wants to start.
* Unnamed Pipes: In contrast to [[Named Pipes]] they can only provide a communication channel between related processes and are strictly unidirectional: e.g. there is a reading and a writing end. By default they are configured to blocking communication which means that implicit synchronization is possible, just like with [[Named Pipes]].

It is often useful to redirect file descriptors (like stdin/stdout/stderr). Redirection works by closing first the file descriptor that should be redirected (e.g. 0 for stdin (POSIX-compliant systems offer symbolic constants (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO) to refer to the std* file descriptors.)) and reopen an existing one (e.g. a file descriptor of a file that is opened in read mode. After the redirection any read from stdin results in a read from the reopened file descriptor). For redirection you need _dup(2)_, _dup2(2)_ and _close(2)_.

## Code Example

The following code example shows a simple program that uses all of the presented mechanisms and functions.

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
 
/* This demo takes two executable file arguments, executes both and connects
 * stdout of the first executable to stdin of the second executable.
 */
 
static const char* my_name = "<unnamed>";
 
static void usage(void)
{
        (void) fprintf(stderr,  "SYNOPSIS: %s <pgm1 ...> <pgm2 ...>\n", my_name);
        (void) fprintf(stderr,  "In case you want to provide arguments to the pgm1 or pgm2 "
                                "just put the whole argument in \"\" eg:\n\t%s \"ls --color\" cat.\n", my_name);
        exit(EXIT_FAILURE);
}
 
static void execute(char* const pgm)
{
        /* this is the exec syscall. it overwrites the current process if successful
         * we start the shell so that we have a set PATH variable 
         */ 
        (void) execlp("/bin/sh", "sh", "-c", pgm, (char *)0); 
 
        /* sh?t happens - if we return, execlp must have failed */
        perror("execlp failed");
        exit(EXIT_FAILURE);     
}
 
static int wait_for_termination(pid_t p)
{
        int status = 0;
 
        if (waitpid(p, &status, 0) == -1) {
                perror("waitpid failed");
        }   
        return WEXITSTATUS(status);
}
 
int main(int argc, char** argv)
{
        pid_t pid_child1, pid_child2;
        char* pgm1 = NULL;
        char* pgm2 = NULL;
        int my_pipe[2];         /* unnamed pipes fd array, see pipe(2). my_pipe[0] is read end,
                                   my_pipe[1] is write end */   
        int stat1 = 0, stat2 = 0;
 
        if (argv[0] && argv[0][0]) {
                my_name = argv[0]; 
        }   
 
        if (argc != 3) {
                usage();
        }
 
        pgm1 = argv[1];
        pgm2 = argv[2];
 
        /* create unnamed pipe
         */
        if (pipe(my_pipe) == -1) {
                perror("cannot create unnamed pipe");
                exit(EXIT_FAILURE);
        }
 
        /* create 2 child processes (siblings, not child and grandchild)
         */
        switch (pid_child1 = fork()) { /* ATT: this also duplicates the unnamed pipe! */
                case -1:
                        perror("error during 1st fork");
                        exit(EXIT_FAILURE);
                        break;
 
                case 0:         /* for the child process, fork(2) returns zero */
                        (void) close(my_pipe[0]);               /* close the read-end of the pipe */
                        (void) close(STDOUT_FILENO);            /* close stdout */
 
                        /* reopen my_pipe[0] -> the lowest fd is taken -> can only be stdout */
                        if(dup(my_pipe[1]) != STDOUT_FILENO)    /* also look into dup2(2) */
                        {
                                perror("cannot reopen write end as stdout");
                        }
                        execute(pgm1);
                        break;  
 
                default:        /* for the father process, fork(2) returns the pid of the child process */                      
                        switch (pid_child2 = fork()) { /* ATT: again - this duplicates the unnamed pipe! */
                                case -1:
                                        perror("error during 2nd fork");
                                        exit(EXIT_FAILURE);
                                        break;
 
                                case 0:         /* for the child process, fork(2) returns zero */
                                        (void) close(my_pipe[1]);               /* close the write-end of the pipe */
                                        (void) close(STDIN_FILENO);             /* close stdin */
 
                                        /* reopen my_pipe[1] -> the lowest fd is taken -> can only be stdin */
                                        if(dup(my_pipe[0]) != STDIN_FILENO)     /* also look into dup2(2) */
                                        {
                                                perror("cannot reopen write end as stdin");
                                        }
                                        execute(pgm2);
 
                                default:        /* for the father process, fork(2) returns the pid of the child process */
                                        break;
 
                        }
                        break;
        }
 
        /* this is the execution path of the parent process 
         * close unnamed pipe: the father doesn't need any of them!
         */
        (void) close(my_pipe[0]);
        (void) close(my_pipe[1]);
 
        /* now collect all exit stati */
        stat1 = wait_for_termination(pid_child1);
        stat2 = wait_for_termination(pid_child2);
 
        (void) fprintf(stdout, "process pid %d (%s) terminated with status: %d\n", pid_child1, pgm1, stat1);
        (void) fprintf(stdout, "process pid %d (%s) terminated with status: %d\n", pid_child2, pgm2, stat2);
        return EXIT_SUCCESS;
}
```

Disclaimer: The information here may contain errors. Please correct them and help to improve this tutorial.

Note: This tutorial is based on multiple sources and your input. It is going to be refined and extended constantly. All of the sources we used are listed below.