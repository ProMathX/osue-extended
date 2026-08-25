## Secure Programming Lint Primer

[splint](http://www.splint.org) is a static analysis tool, which assists programmers in verifying the correctness of their C programs, finding errors and ensuring the absence of common mistakes. The goal of this introduction is to give you an idea of what splint is, what it is not, and how you can make efficient use of this tool.

splint fits very well into the safe programming requirements of systems programming. However, there are some key differences to more conservative checks, like the ones produced by the compiler (<tt>gcc -Wall</tt>):

*   You need to understand your code very well to decide whether a <tt>splint</tt> warning needs to be fixed or not. This is a good thing: Checking the correctness of a program requires you, the programmer, to think about it carefully.

*   You need to decide which checks make sense for your program. Depending on your coding style, some checks might produce to many false positives. Other checks need annotations (special comments) to work reasonably well. Initially, try <tt>splint -weak</tt>, which only runs checks with a low false positive rate.

The example below illustrates these key points.

### Configuration

Be aware that you will need to configure splint (using a few command line options) to meet your requirements.

The default options are configured in <tt>~/.splintrc</tt>. Initially, we put the following options in there:

*   Options for handling systemfiles: We use posix libraries (`+posixlibs`), our system files are in /usr/include (`-sysdirs /usr/include`) and we are not interested in warnings in system header files (`-warnsysfiles`).

*   Options for improved compatibility with a C89-based coding style: We do not distinguish between bools and ints (`+boolint`). The bool datatype was only introduced in C99.

*   Options turning off checks which are not interesting for us (`-exportlocal`).

The `~/.splintrc` configuration file thus initially looks like this:

```
-warnsysfiles +posixlib -sysdirs /usr/include/ +boolint -exportlocal
```

You might want to add more options to this file later on.

### Checking

We want to analyze the following example code in `splint_demo.c`, which compiles fine, and looks ok at a first glance.

```
   /*  1 */  #include <stdio.h>
   /*  2 */  #include <string.h>
   /*  3 */  int is_long_arg(char* s) {
   /*  4 */      return strlen(s) > 32;
   /*  5 */  }
   /*  6 */  int main(int argc, char** argv) {
   /*  7 */      int i, r;
   /*  8 */      char* arg = NULL;
   /*  9 */      --argc; ++argv; /* ignore program name */
   /* 10 */      for(i = 0; i < argc; i++) {
   /* 11 */          arg = argv[i];
   /* 12 */          r = is_long_arg(arg);
   /* 13 */          if(r) break;
   /* 14 */      }
   /* 15 */      if(r) printf("Found long arg: %s\n",arg);
   /* 16 */      else  printf("No long argument: :(\n");
   /* 17 */      return 0;
   /* 18 */  }
```

If we invoke splint via `splint -weak splint_demo.c` we get no warnings, so the conservative static checks did not find a problem.

Next, we run `splint splint_demo.c` which reports two warnings. The first one is

```
splint_demo.c:15:21: Variable r used before definition
   An rvalue is used that may not be initialized to a value on some execution
   path. (Use -usedef to inhibit warning)
```

The first warning corresponds to a bug in our program: if there are no arguments, `r` will be uninitialized at line 15, and so the then branch might be executed by pure chance. Fixing this bug and initializing `r` to `0` leaves us with the other warning:

```
splint_demo.c:15:55: Null storage arg passed as non-null param: printf (..., arg, ...)
   A possibly null pointer is passed as a parameter corresponding to a formal
   parameter with no /*@null@*/ annotation...
```

This is an example of a spurios warning - splint was not able to understand that, if `r` is non-zero, arg will always be defined. The good thing is, it required us think about this potential problem. Once we are sure that arg will never be NULL at line 15, we can document this fact in our code. The splint manual recommends to use a assertion for this purpose:

```
   /* 15 */      if(r) {
                           assert(arg != NULL); 
                           printf("Found long arg: %s\n",arg);
                        }
```

The assertion also serves as documentation and can be used as a starting point for formal verification (e.g., using [http://www.cprover.org/cbmc/](http://www.cprover.org/cbmc/)). You need to decide yourself, however, whether it is worth putting in assertions or not.

This concludes this small introductionary example - be sure to check out the [splint manual](http://www.splint.org/manual/manual.html) to learn more about using splint.

## Experiences & Examples

Space for examples and experiences with splint

splint is a very useful and powerful tool to debug your code and get most things right. Even if it is sometimes a bit caring about what you do (declaring pointers to an int seems to be a must-be for splint, so passing &int to a function is not ok.). Working with splint is also fun, you may call it that, because it really helps you get things right in your code. It is not bloated nor to thin. Some semantic glitches are not detected, but hey, you should know at all points what your program is doing. As stated above, you should also know when you should implement splints advice and when not. It will tell you, that you didn't check the return of close, but if you are performing a 'full-stop' in your program, because something really went wrong, that's not the right place to start checking returns of functions. Then it is probably too late. All in all, I recommend using splint. It tells you about gaping holes in your software and knows much about the C way of doing things. Only thing that is a bit annoying, (im on Ubuntu x64 10.04), is that it sometimes tells me that functions that I imported are not known and can't be used at this point, where the gcc doesn't warn me at all.