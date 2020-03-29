# Skeleton for building a ELF loader

## Introduction
This project contains a skeleton for building an
[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) binary
on-demand loader in Linux. The loader will provide two methods, defined in the
`loader.h` header:
* `int so_init_loader(void);` - initializes the on-demand loader
* `int so_execute(char *path, char *argv[]);` - executes the binary located in
`path` with the required `argv` arguments.

## Content
The project contains of three components, each of them in its own
directory:
* `loader` - a dynamic library that can be used to run ELF binaries. It
consists of the following files:
  * `exec_parser.c` - Implements an ELF binary parser.
  * `exec_parser.h` - The header exposed by the ELF parser.
  * `loader.h` - The interface of the loader, described in the
  [Introduction](#introduction) section.
  * `loader.c` - This is where the loader should be implemented.
  * `debug.h` - header for the `dprintf` function that can be used for logging
  and debugging.
* `exec` - a program that uses the `libso_loader.so` library to run an ELF
binary received as argument.
* `test_prog` - an ELF binary used to test the loader implementation.

There project also contains 2 makefiles:
* `Makefile` - builds the `libso_loader.so` library from the `loader`
directory
* `Makefile.example` - builds the `so_exec` and `so_test_prog` binaries from
the `exec` and `test_prog` directories that can be used to test the loader.

## Usage Build the loader:
```
make
```

This should generate the `libso_loader.so` library. Next, build the example:

```
make -f Makefile.example
```

This should generate the `so_exec` and `so_test_prog` used for the test:

```
LD_LIBRARY_PATH=. ./so_exec so_test_prog
```

**NOTE:** the skeleton does not have the loader implemented, thus when running
the command above, your program will crash!

## Notes
This skeleton is provided by the Operating System team from the University
Politehnica of Bucharest to their students to help them complete their
Executable Loader assignment.
