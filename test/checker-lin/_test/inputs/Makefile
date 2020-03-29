CC = gcc
BINS = hello sum bss data_bss no_perm inv_perm check_perm check_exec qsort
CFLAGS = -m32 -Wall -fno-pic -fno-stack-protector -g
LDFLAGS = -m32 -static -no-pie

all: $(BINS)

hello: hello.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $<

hello.o: hello.S
	$(CC) -m32 -o $@ -c $<

sum: sum.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

sum.o: sum.c
	$(CC) $(CFLAGS) -o $@ -c $<

bss: bss.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

bss.o: bss.c
	$(CC) $(CFLAGS) -o $@ -c $<

data_bss: data_bss.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

data_bss.o: data_bss.c
	$(CC) $(CFLAGS) -o $@ -c $<

no_perm: no_perm.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

no_perm.o: no_perm.c
	$(CC) $(CFLAGS) -o $@ -c $<

inv_perm: inv_perm.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

inv_perm.o: inv_perm.c
	$(CC) $(CFLAGS) -o $@ -c $<

check_perm: check_perm.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

check_perm.o: check_perm.c
	$(CC) $(CFLAGS) -o $@ -c $<

check_exec: check_exec.o lib.o
	$(CC) $(LDFLAGS) -nostdlib -o $@ $^

check_exec.o: check_exec.c
	$(CC) $(CFLAGS) -o $@ -c $<

qsort: qsort.o
	$(CC) $(LDFLAGS) -o $@ $^

qsort.o: qsort.c
	$(CC) $(CFLAGS) -o $@ -c $<

lib.o: lib.c
	$(CC) $(CFLAGS) -O2 -o $@ -c $<

clean:
	-@rm -f hello.o hello
	-@rm -f sum.o sum
	-@rm -f bss.o bss
	-@rm -f data_bss.o data_bss
	-@rm -f no_perm no_perm.o
	-@rm -f inv_perm inv_perm.o
	-@rm -f check_perm check_perm.o
	-@rm -f check_exec check_exec.o
	-@rm -f qsort qsort.o
	-@rm -f lib.o
