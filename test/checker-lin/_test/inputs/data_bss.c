int __attribute__((aligned(4096))) arr_init[512] = { 0xdeadbeef };
int arr_uninit[1024];

void exit(int code);
int write(int fd, char *buf, int len);

void _start()
{
	int i;

	/* check initialized data */
	if (arr_init[0] != 0xdeadbeef) {
		write(1, "FAIL: initialized data\n", 23);
		exit(1);
	} else {
		/* check uninitialized data */
		for (i = 0; i < 1024; i++)
			if (arr_uninit[i] != 0) {
				write(1, "FAIL: bss\n", 10);
				exit(1);
			}
	}

	write(1, "SUCCESS\n", 8);

	exit(0);
}
