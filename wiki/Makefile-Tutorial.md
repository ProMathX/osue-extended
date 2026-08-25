_Make_ is a powerful dependency tracking based tool that allows you to manage your builds from source code. Not just C-code, but any source code. When you modify (usually) a small part of your project and have followed good development practice, _make_ will _know_ what it needs to rebuild and shortens build times dramatically -- especially for large projects. For the rest of this tutorial we will focus on C programs and their peculiarities.

* * *

**example:** Imagine a small C program that consists of a couple of modules and a header file that each module includes for commonly used constants. When you modify one of the *.c source files, the c-module needs to be recompiled (usually to a *.o object file), while when you modify the *.h header file, all the c-modules that include that header file in their *.c source file must be recompiled too. This is just to ensure that all changes are considered for the final output of the build. Ultimately all object files are linked to the C program executable: Again this linking step must be done if any module has been changed.

* * *

The tool _make_ looks - when executed - for a file called _Makefile_ in the same directory. The _Makefile_ contains the configuration which consists of _rules_. A _rule_ looks very similar to this:

```
<target 1> <target 2> ... <target n> : <prerequisite 1> <prerequisite 2> ... <prerequisite n>
[tab]<recipe 1>
[tab]<recipe 2>
[tab].
[tab].
[tab].
[tab]<recipe n>
```

The targets can be files or goals. If it is a file, it is supposed to be built by the _recipe_ lines if and only if it is older than one of the prerequisites. As the shape indicates: multiple targets can be built by the same rule. Also the target/s may depend on multiple prerequisites (e.g. source or object files). A recipe simply executes the contents of the line within the current directory. By default a recipe line is introduced with the special character '\t' (tab). Note that we do not print [tab] from now on in the code snippets, but use actual tabs.

* * *

**example:** Depending on the name of the executable (argv[0]) the same program is used to print "hello world" and "bye world". Both executables require the object file 'main.o' which in return requires the source file 'main.c' and 'constants.h'. In case the source or header file is modified, 'make' will execute the commands listed in the recipe lines of the target 'main.o'. This also triggers the rule above, because main.o is a prerequisite for both the executables helloworld and byeworld (chained dependencies).

```
helloworld byeworld: main.o
	gcc main.o -o helloworld
	gcc main.o -o byeworld
 
 
main.o: main.c constants.h
	gcc -ansi -pedantic -Wall -c main.c
```

* * *

Goals (i.e. targets that are no files) are useful to define small batch scripts (one or multiple recipe lines) that are always executed when requested. For example there are some default goals that implement functionality any build system must support: clean and all. _clean_ is supposed to remove any generated file and literally cleans the project directory, while _all_ should build all of the project's targets (e.g. executables, libraries). To indicate that a goal is a goal and not a target for a file, the goal can be marked as a phony target: `.PHONY: all clean`. That way, _make_ will not even look for a file 'all' or 'clean' within the directory it is called (which prevents special cases where you actually have a file called 'all' or 'clean' in your project directory and are puzzled why a 'make all' does not seem to build your project anymore).

* * *

**example:**

```
.
.
.
 
.PHONY : all clean
 
all: myprogram1 myprogram2 myprogram3
 
 
clean:
	rm -f myprogram1 myprogram2 myprogram3 *.o
```

* * *

Now _make_ called without any parameters will try to build the first target or goal defined in the first rule of the 'Makefile' (which is not a pattern rule or starts with a dot (.) ). It's possible to override this behavior by specifying the target(s) or goal(s) when calling

```
make: make <first target/goal> <second target/goal> ...
```

* * *

**example:** This call will first clean a project and then build all the defined executables, libraries, etc.

```
make clean all
```

* * *

It is also possible to define variables (sometimes called macros) in 'Makefiles'. A variable is essentially a name that is substitutable with a string of text. Variables can be used after their definition at many locations within the 'Makefile' (e.g. targets, prerequisites and within recipes). A variable name is treated case sensitive and should only contain letters and numbers.

* * *

**example:**

```
# note that spaces between the define/assign operator and the first non-space character are ignored!
# hint: don't do this in real Makefiles - a real world example will be given at the end of the tutorial
OBJ_SUFX = .o
OBJS = main.$(OBJ_SUFX) foo.$(OBJ_SUFX) bar.$(OBJ_SUFX) utils.$(OBJ_SUFX)
 
executable : $(OBJS)
	gcc -o executable $(OBJS)
```

* * *

## 'Real World' examples

The following examples show some relatively simple 'Makefiles' that you can use as a starting point for the exercises. You may modify the presented examples or add new ones, but please keep in mind: This is supposed to help you (and your fellow students) to understand how _make_ works. We also expect you to be perfectly able to explain your submitted 'Makefiles'.

### Simple Makefile

Simple as this 'Makefile' may seem it already contains all the mechanisms that you need to know for the course including the exam.

```
CFLAGS = -ansi -pedantic -Wall -c -g -D_XOPEN_SOURCE=500
LFLAGS =
 
all: myprogram
 
myprogram : module1.o module2.o module3.o module4.o
	$(CC) $(LFLAGS) -o myprogram module1.o module2.o module3.o module4.o 
 
module1.o: module1.c global.h module1.h
	$(CC) $(CFLAGS) module1.c
.
.
.
module4.o: module4.c global.h module4.h
	$(CC) $(CFLAGS) module4.c
 
 
clean:
	rm -f myprogram
```

### 'Makefile' for single executable projects

```
CC=gcc
CFLAGS=-ansi -pedantic -Wall -D_XOPEN_SOURCE=500
LFLAGS=
 
CFILES	= module1.c module2.c module3.c module4.c
PGMNAME	= myprogram
 
OBJ	= $(CFILES:.c=.o)    # traverse a space separated list of strings and replace the suffix .c with .o
                         # e.g. module1.c module2.c module3.c module4.c is transformed to
                         # module1.o module2.o module3.o module4.o
DFILES	= $(CFILES:.c=.d)
NODEPS	= clean
 
all: $(PGMNAME)
 
 
# the following construct is a workaround to check whether the currently requested make target
# is within the list of the NODEPS variable. We designed this Makefile to not create automatic 
# dependency files for certain configurable targets: e.g. clean
 
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
  -include $(DFILES)          # slient fail include: if the *.d files are not existing, make will not
                              # abort execution 
endif
 
# pattern rules
# They contain a lot of 'magic':
# -) patterns: %.suffix in the targets list matches on any file target that ends with the suffix
#    when put in the prerequisites list, the % is replaced by the target prefix: e.g. %.d matches on
#    module1.d and the pattern rule is interpreted as: module1.d : module1.c
# -) automatic variables:
#    $< is the first element of the prerequisite list
#    $@ is the first element in the target list
# Note: > is just a stdout redirect and the parameter -MM instructs gcc to generate rules for 'make'
# by parsing the C file for locally included header files: e.g. #include "header.h" are considered 
# while #include <header.h> are ignored.       
 
%.d: %.c
	$(CC) $(CFLAGS) -MM $< > $@
 
%.o: %.c %.d %.h
	$(CC) $(CFLAGS) -o $@ -c $<
 
 
clean:
	rm -f $(PGMNAME) $(OBJ) $(DFILES)
 
$(PGMNAME): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $@
 
.PHONY: clean all
```

## Links

Other _make_ tutorials (if you find another good one: please add it!):

* [http://mrbook.org/tutorials/make/](http://mrbook.org/tutorials/make/)
* [http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/](http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/)
* [http://www.gnu.org/software/make/manual/make.html](http://www.gnu.org/software/make/manual/make.html)
* [http://www.ijon.de/comp/tutorials/makefile.html](http://www.ijon.de/comp/tutorials/makefile.html) (german)

Disclaimer: The information here may contain errors. Please correct them and help to improve this tutorial.

Note: This tutorial is based on multiple sources and your input. It is going to be refined and extended constantly. All of the sources we used are listed below. Note: For the course we will be using a recent GNU make version. We will not cover any advanced mechanisms like (most of the) built-in functions, recursive builds, or secondary expansions.