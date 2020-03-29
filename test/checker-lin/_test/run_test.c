/*
 * Operating System Executable Loader Tester
 *
 * 2018, Operating Systems
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#include "loader.h"

static int no_page_faults;
static char *fault_count_fname;
static void (*previous_handler)(int, siginfo_t *, void *);

static int itoa(int n, char *buf)
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

	while (n) {
		c = n % 10;
		n /= 10;
		buf[i] = '0' + c;
		i--;
	}

	return num_digits;
}

static void test_segv_handler(int signum, siginfo_t *info, void *context)
{
	int fd;
	char buf[128];
	int len;

	if (signum != SIGSEGV)
		return;
	if (info->si_signo != SIGSEGV)
		return;

	if (info->si_code == SEGV_MAPERR)
		no_page_faults++;

	fd = open(fault_count_fname, O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (fd >= 0) {
		len = itoa(no_page_faults, buf);
		buf[len++] = '\n';
		write(fd, buf, len);
		close(fd);
	}

	previous_handler(signum, info, context);
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	struct sigaction prev;

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (argc == 3)
		fault_count_fname = argv[2];
	else
		fault_count_fname = "fault_cnt.out";

	so_init_loader();

	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = test_segv_handler;
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGSEGV, &sa, &prev) < 0) {
		fprintf(stderr, "cannot get previous signal handler\n");
		return 1;
	}

	if ((void(*)())prev.sa_sigaction == SIG_DFL) {
		fprintf(stderr, "so_init_loader didn't set a signal handler\n");
		return 1;
	}
	previous_handler = prev.sa_sigaction;

	so_execute(argv[1], argv+1);

	// shouldn't reach here
	return 1;
}
