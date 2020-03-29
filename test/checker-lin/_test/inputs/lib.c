int write(int fd, char *buf, int len)
{
	int rc;

	asm volatile(
		"int $0x80 \n"
		:"=a"(rc)
		:"a"(4), "b"(fd), "c"(buf), "d"(len)
		);
	return rc;
}

int read(int fd, char *buf, int len)
{
	int rc;

	asm volatile(
		"int $0x80 \n"
		:"=a"(rc)
		:"a"(3), "b"(fd), "c"(buf), "d"(len)
		);
	return rc;
}

int open(char *path, int flags, int mode)
{
	int rc;

	asm volatile(
		"int $0x80 \n"
		:"=a"(rc)
		:"a"(5), "b"(path), "c"(flags), "d"(mode)
		);
	return rc;
}

int close(int fd)
{
	int rc;

	asm volatile(
		"int $0x80 \n"
		:"=a"(rc)
		:"a"(6), "b"(fd)
		);
	return rc;
}

void exit(int code)
{
	while (1) {
		asm volatile(
			"int $0x80 \n"
			"hlt \n"
			:
			:"a"(1), "b"(code)
			);
	}
}

int itoa(int n, char *buf)
{
	int num_digits = 0;
	int n2 = n;
	int i;
	int c;

	while (n) {
		n /= 10;
		num_digits++;
	}

	n = n2;
	i = num_digits-1;

	while(n) {
		c = n % 10;
		n /= 10;
		buf[i] = '0' + c;
		i--;
	}

	return num_digits;
}
