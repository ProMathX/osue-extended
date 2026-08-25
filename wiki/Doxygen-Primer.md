In this course we use Doxygen to document the sourcecode. While there are many methods [1] to document, we believe that Doxygen is currently one of the most flexible, feature rich (w.r.t. languages) and easy to use documentation tools. In fact you only need to

* place comments into your code in a specific intuitive way/style (e.g. Doxygen knows Javadoc style)
* make minimal adjustments to a default configuration (or major, in case you: fancy different layouts, require multiple _views_ on your documentation, ...)
* and finally run the doxygen tool to generate html, latex, rtf, xml and/or Unix man pages.

## Getting Started

In the beginning, it's best to agree on a directory structure of your project. Within this course you probably don't need more than a simple one like:

<pre>
taskname
     |----src
     |
     \----doc
</pre>

You can create a default configuration that produces html and Latex output by running:

<pre>
# cd taskname
# doxygen -g ./doc/Doxyfile
</pre>

Then, edit the ./doc/Doxyfile and adjust the parameter _INPUT_. For the suggested directory structure you should change it to:

<pre>
INPUT             =  ../src
</pre>

Additionally change FILE_PATTERNS to include source files and header files and enable EXTRACT_STATIC to show the documentation of static functions and variables.

<pre>
FILE_PATTERNS     = *.c *.h
EXTRACT_STATIC    = YES
</pre>

After that you may start programming your task. The following example consists of 2 modules, whereas the main module uses the util module for printing to stdout.

### main.c

```
/**
 * @file main.c
 * @author Bernhard Froemel <froemel@vmars.tuwien.ac.at>
 * @date 10.03.2011
 *
 * @brief Main program module.
 * 
 * This program demonstrates Doxygen and prints among a few unimportant lines of ASCII text
 * "Hello World" out on stdout.
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
 
static char *pgm_name; /**< The program name. This comment just shows you how to comment (member) variables/constants or defines on the same line */
 
/**
 * Mandatory usage function.
 * @brief This function writes helpful usage information about the program to stderr.
 * @details global variables: pgm_name
 */
static void usage(void) {
	(void) fprintf(stderr, "USAGE: %s\n", pgm_name);
	exit(EXIT_FAILURE);
}
 
/**
 * Program entry point.
 * @brief The program starts here. This function takes care about parameters, and if the
 * overall program is reasonable trivial it could implement the whole program functionality -
 * like here. On topic (you should not state such general (coding style) information in your 
 * documentation): This function writes a few lines of text including "Hello World" to stdout. 
 * @details Try to keep the main function small and move any functionality that is used more 
 * than  a single time to extra function(s). Note that you should restrict visibility of those
 * extra functions to the smallest required scope (start with static).
 * global variables: pgm_name
 * @param argc The argument counter.
 * @param argv The argument vector.
 * @return Returns EXIT_SUCCESS.
 */
int main(int argc, char **argv) {
	pgm_name = argv[0];
 
	if(argc != 1) {
		usage();
	}
 
	println("This is the first line.");
	println("This is the second line.");
	println("Wait for it...");
	println("Hello World!");
 
	return EXIT_SUCCESS;
}
```

### util.h

```
/**
 * @file util.h
 * @author Bernhard Froemel <froemel@vmars.tuwien.ac.at>
 * @date 10.03.2011
 *  
 * @brief Provides utility functions useful for programs.
 *
 * The util module. It contains generic functions related to standard in and output.
 */
 
#ifndef UTIL_H__ /* prevent multiple inclusion */
#define UTIL_H__
 
/**
 * Print a line to stdout.
 * @brief This function prints the current line number and a given string (line parameter) 
 * to stdout followed by a newline.
 * @details The function does not check whether the input line already contains newlines. So the 
 * line counter works only correctly if there are no newlines contained in the line string.
 * @param line This string is printed to stdout.
 */
void println(char* const line);
 
 
#endif /* UTIL_H__ */
```

### util.c

```
/**
 * @file util.c
 * @author Bernhard Froemel <froemel@vmars.tuwien.ac.at>
 * @date 10.03.2011
 * 
 * @brief Implementation of the util module.
 *
 * While the header file contains information useful for developers who *use* this module,
 * the implementation file contains information for developers who extend/maintain the module.
 */
#include <stdio.h>
#include "util.h"
 
/** Current line counter. 
 * @brief By default you need to place the doxygen comment before the (member) variable/constant/define you want to document.
 */
static int cur_line_cnt = 1;
 
/**
 * @details globals: cur_line_cnt, visibility restricted to util module.
 */
void println(char* const line) {
	fprintf(stdout, "%u: %s\n", cur_line_cnt, line);
	cur_line_cnt++;
}
```

## Links

[1] [http://www.naturaldocs.org/](http://www.naturaldocs.org/) [http://haskell.org/haddock/](http://haskell.org/haddock/) [http://docpp.sourceforge.net/](http://docpp.sourceforge.net/) [http://www.oracle.com/technetwork/java/javase/documentation/index-jsp-135444.html](http://www.oracle.com/technetwork/java/javase/documentation/index-jsp-135444.html)

[http://www.stack.nl/~dimitri/doxygen/starting.html](http://www.stack.nl/~dimitri/doxygen/starting.html)