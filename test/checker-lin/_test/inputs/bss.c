int __attribute__((aligned(4096))) arr[1024];

void exit(int code);
int write(int fd, char *buf, int len);

void _start()
{
	int i;

	for (i = 0; i < 1024; i++)
		if (arr[i] != 0) {
			write(1, "FAIL\n", 5);
			exit(1);
		}

	write(1, "SUCCESS\n", 8);
	exit(0);
}
