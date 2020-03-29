BINS = check_exec.exe hello.exe sum.exe bss.exe data_bss.exe no_perm.exe inv_perm.exe check_perm.exe qsort.exe
LDFLAGS = /nologo /dynamicbase:no /fixed /base:0x10000000 /align:65536 /INCREMENTAL /IGNORE:4108

all: build

build: $(BINS)

check_exec.exe: check_exec.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

check_exec.obj: check_exec.asm
	ml /nologo /c /Fo$@ $**

hello.exe: hello.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

hello.obj: hello.asm
	ml /nologo /c /Fo$@ $**

sum.exe: sum.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

sum.obj: sum.c
	cl /nologo /c /GS- /Fo$@ $**

bss.exe: bss.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

bss.obj: bss.c
	cl /nologo /c /GS- /Fo$@ $**

data_bss.exe: data_bss.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

data_bss.obj: data_bss.c
	cl /nologo /c /GS- /Fo$@ $**

no_perm.exe: no_perm.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

no_perm.obj: no_perm.c
	cl /nologo /c /GS- /Fo$@ $**

inv_perm.exe: inv_perm.obj
	link $(LDFLAGS) /out:$@ /entry:start /subsystem:console $** kernel32.lib

inv_perm.obj: inv_perm.c
	cl /nologo /c /GS- /Fo$@ $**

check_perm.exe: check_perm.obj
	link $(LDFLAGS) /out:$@ $**

check_perm.obj: check_perm.c
	cl /nologo /c /MTd /Fo$@ $**

qsort.exe: qsort.obj
	link $(LDFLAGS) /out:$@ $**

qsort.obj: qsort.c
	cl /nologo /c /MTd /Fo$@ $**

clean:
	del /Q check_exec.obj 2>NUL
	del /Q check_exec.exe 2>NUL
	del /Q check_exec.ilk 2>NUL
	del /Q hello.obj 2>NUL
	del /Q hello.exe 2>NUL
	del /Q hello.ilk 2>NUL
	del /Q sum.obj 2>NUL
	del /Q sum.exe 2>NUL
	del /Q sum.ilk 2>NUL
	del /Q bss.obj 2>NUL
	del /Q bss.exe 2>NUL
	del /Q bss.ilk 2>NUL
	del /Q data_bss.obj 2>NUL
	del /Q data_bss.exe 2>NUL
	del /Q data_bss.ilk 2>NUL
	del /Q no_perm.obj 2>NUL
	del /Q no_perm.exe 2>NUL
	del /Q no_perm.ilk 2>NUL
	del /Q inv_perm.obj 2>NUL
	del /Q inv_perm.exe 2>NUL
	del /Q inv_perm.ilk 2>NUL
	del /Q check_perm.obj 2>NUL
	del /Q check_perm.exe 2>NUL
	del /Q check_perm.ilk 2>NUL
	del /Q qsort.obj 2>NUL
	del /Q qsort.exe 2>NUL
	del /Q qsort.ilk 2>NUL
