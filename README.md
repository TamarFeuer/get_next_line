<h1 align="center">
	📖 get_next_line
</h1>

<p align="center">
	<b><i>Reading a line on a fd is way too tedious</i></b><br>
</p>

<h3 align="center">
	<a href="#%EF%B8%8F-about">About</a>
	<span> · </span>
	<a href="#%EF%B8%8F-usage">Usage</a>
	<span> · </span>
	<a href="#-testing">Testing</a>
</h3>

---

## 💡 About the project

> _a function that returns a line read from a file descriptor_

get_next_line project is a function that reads a file descriptor line by line until the end of the file or an error occurs. 
The function is designed to be used with the standard input, a file or a network connection. 
The purpose of this project is to learn how to use static variables, and file descriptors in C programming.
The function should be capable of reading and returning lines from different file descriptors. 
If the stack memory is used, the function will most likely break for very high BUFFER_SIZE values and very large lines, due to stack overflow.
So the design should make use of either the data segment memory in which static variables are saved, or the dynamically allocated memory on the heap.


## 🛠️ Usage

### Requirements

The function is written in C language and thus needs the **`gcc` compiler** and some standard **C libraries** to run.

### Instructions

**1. Using it in your code**

To use the function in your code, simply include its header:

```C
#include "get_next_line.h"
```

and, when compiling your code, add the source files and the required flag:

```shell
get_next_line.c get_next_line_utils.c -D BUFFER_SIZE=<size>
```

## 📋 Testing

You only have to edit the get_next_line.c file and uncomment the main function and headers inside it.
You can edit test.txt files to put another text if you wish to test othe cases.
Then simply run this command (change "xx" with desired buffer size) :

```shell
cc -Wall -Werror -Wextra -D BUFFER_SIZE=xx get_next_line.c get_next_line_utils.c && ./a.out
```
