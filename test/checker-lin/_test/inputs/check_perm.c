#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void exit(int);

int x;

#define BUF_SIZE 8192

static char *strchr(const char *s, int c)
{
	int i;

	for (i = 0; s[i]; i++)
		if (s[i] == c)
			return (char *)(s+i);

	return NULL;
}

unsigned long strtoul(char *s, char **endptr)
{
	unsigned long val = 0;
	int negative = 0;

	if (*s == '-') {
		negative = 1;
		s++;
	}

	if (*s == '0' && ((*s == 'x') || (*s == 'X')))
		s += 2;

	while (1) {
		if (!*s)
			break;

		if (*s >= '0' && *s <= '9')
			val = (val << 4) + (*s - '0');
		else if (*s >= 'a' && *s <= 'f')
			val = (val << 4) + (*s - 'a' + 10);
		else if (*s >= 'A' && *s <= 'F')
			val = (val << 4) + (*s - 'A' + 10);
		else
			break;

		s++;
	}

	*endptr = s;

	if (negative)
		val = -val;

	return val;
}

int virtual_query(void *addr, int *perm)
{
	char buf[BUF_SIZE];
	uintptr_t __addr, start, end;
	char *p;
	char *q;
	char *line;
	int fd;
	int found = 0;
	int ret = 0;
	int rc;

	*perm = 0;

	fd = open("/proc/self/maps", O_RDONLY);
	if (fd < 0) {
		write(1, "Couldn't open /proc/self/maps\n", 30);
		goto out;
	}

	rc = read(fd, buf, BUF_SIZE);
	if (rc < 0) {
		write(1, "Read error\n", 11);
		goto out_close;
	}

	__addr = (uintptr_t)addr;
	line = buf;

	/*
	 * a line looks like this:
	 * 08048000-080ea000 r-xp 00000000 08:01 6555842 ...
	 */
	while(1) {
		p = strchr(line, '-');
		if (!p) {
			write(1, "Error parsing /proc/self/maps\n", 30);
			goto out_close;
		}

		start = strtoul(line, &q);
		if (q != p) {
			write(1, "Error parsing /proc/self/maps\n", 30);
			goto out_close;
		}

		end = strtoul(p+1, &q);
		if (*q != ' ') {
			write(1, "Error parsing /proc/self/maps\n", 30);
			goto out_close;
		}

		q++;
		if (__addr >= start && __addr < end) {
			found = 1;
			break;
		}

		line = strchr(line, '\n');
		if (!line)
			break;
		line++;
	}

	if (!found)
		goto out_close;

	if (q[0] == 'r')
		*perm |= PROT_READ;
	if (q[1] == 'w')
		*perm |= PROT_WRITE;
	if (q[2] == 'x')
		*perm |= PROT_EXEC;

	ret = 1;

out_close:
	close(fd);
out:
	return ret;
}

void _start()
{
	int perm;
	int ret;

	// make sure x is loaded into memory
	x = 200;

	// query an address inside the code area
	ret = virtual_query(&_start, &perm);
	if (!ret) {
		write(1, "virtual_query failed\n", 21);
		exit(1);
	}

	// the permissions should be rx
	if (perm != (PROT_READ|PROT_EXEC)) {
		write(1, "Code area doesn't have rx permissions\n", 38);
		exit(1);
	}

	// query an address inside the data area
	ret = virtual_query(&x, &perm);
	if (!ret) {
		write(1, "virtual_query failed\n", 21);
		exit(1);
	}

	// the permissions should be rw
	if (perm != (PROT_READ|PROT_WRITE)) {
		write(1, "Data area doesn't have rw permissions\n", 38);
		exit(1);
	}

	write(1, "SUCCESS\n", 8);

	exit(0);
}
