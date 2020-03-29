CC = gcc
CFLAGS = -m32 -Wall -fno-pic
LDFLAGS = -m32 -no-pie
LDLIBS = -lso_loader

.PHONY: build
build: run_test
	$(MAKE) -C inputs

run_test: run_test.o
	$(CC) $(LDFLAGS) -L.. -Wl,-Ttext-segment=0x20000000 -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	-@rm -f run_test.o run_test
	$(MAKE) -C inputs clean
