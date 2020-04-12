<p align="center">
  <a href="" rel="noopener">
 <img width="25%" height="25%" src="res/ubuntu-logo.png" alt="Project logo"></a>
</p>

<h1 align="center">E.L.F Executable Loader</h3>

<p align="justify"> 
The E.L.F Executable Loader loads the executable file into memory page-by-page, using a request-paging mechanism - a page will only be loaded when it is needed. For simplicity, the loader runs only static executable files - which is not linked to shared / dynamic libraries.
    <br> 
</p>

## 📝 Table of Contents
- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)
- [Built Using](#built_using)
- [Contributing](../CONTRIBUTING.md)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>
The interface of the loader is presented in the header loader.h file. This has functions to initialize a loader (so_init loader) and execute a binary (so_execute)

* The so_init_loader function performs the library initialization. Within the function, the page fault action will be a routine for handling the **SIGSEGV** signal.
* The so_execute function performs the parsing of the binary specified in the path and executes the first entry point of the executable.

## 🏁 Getting Started <a name = "getting_started"></a>
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
To use the loader.h library on your projects you must have:

* for UNIX based operating systems:
  * gcc - is a tool from the GNU Compiler Collection used to compile and link C programs

### Installing
This is a step by step series of examples that tell you how to get a development env running.

* Linux:
  * start by updating the packages list
    ```bash
    $sudo apt-get update
    ```
  * install the build-essential package(a package of new packages including gcc, g++ and make) by typing:
    ```bash
    $sudo apt-get install build-essential 
    ```
## 🔧 Running the tests <a name = "tests"></a>
If you want to run the automated tests for Linux system you must follow the following steps:
* clone the repository by copping the following command in your terminal:
  ```
  git clone https://github.com/RusuGabriel/E.L.F-Executable-Loader.git
  ```
* go into the project director and run the following command:
  ```bash
  make test
  ```
* the results of the tests will be in the newly created file result.out

## 🎈 Usage <a name="usage"></a>
If you want to use the ***libso_loader.so*** library in your projects then you must add the ***loader.h*** header in the desired source file and specify at the compile time the path to the libso_loader.so library.

* running the following command in the project director will generate the libso_loader.so:
  ```bash
  make build
  ```
## ⛏️ Built Using <a name = "built_using"></a>
- [Visual Studio Code](https://code.visualstudio.com/) - code editor
- [gcc](https://gcc.gnu.org/) - used to compile the library on my Linux machine

## ✍️ Authors <a name = "authors"></a>
- [@RusuGabriel](https://github.com/RusuGabriel) - implementation
