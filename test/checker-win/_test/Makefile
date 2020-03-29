CFLAGS = /nologo /W3 /EHsc
CC = cl
LINK = link
LDFLAGS = /nologo /DYNAMICBASE:NO /FIXED /INCREMENTAL

all: build

build: run_test.exe
	cd inputs && nmake /nologo all && cd ..

run_test.exe: run_test.obj
	$(LINK) $(LDFLAGS) /BASE:0x20000000 /OUT:$@ $** ../so_loader.lib

run_test.obj: run_test.c
	$(CC) $(CFLAGS) /Fo$@ /c $**

clean:
	del /Q run_test.obj 2>NUL
	del /Q run_test.exe 2>NUL
	del /Q run_test.ilk 2>NUL
	cd inputs && nmake clean && cd ..
