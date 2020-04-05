/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "exec_parser.h"

static so_exec_t *exec;
static int exec_decriptor;

ssize_t xread(int fd, void *buf, size_t count)
{
	size_t bytes_read = 0;

	while (bytes_read < count)
	{
		ssize_t bytes_read_now =
			read(fd, buf + bytes_read,
				 count - bytes_read);

		if (bytes_read_now == 0) /* EOF */
			return bytes_read;

		if (bytes_read_now < 0) /* I/O error */
			return -1;

		bytes_read += bytes_read_now;
	}

	return bytes_read;
}

void copy_into(so_seg_t *segment, int offset)
{
	char *buffer = calloc((segment->mem_size - offset), sizeof(char));
	lseek(exec_decriptor, segment->offset, SEEK_SET);
	xread(exec_decriptor, buffer, segment->file_size - offset);
	memcpy(segment->data, buffer, (segment->mem_size - offset));
}

so_seg_t *find_segment_of(void *addr)
{
	for (int i = 0; i < exec->segments_no; i++)
		if ((char *)addr - (char *)exec->segments[i].vaddr < exec->segments[i].mem_size)
			return &(exec->segments[i]);
	return NULL;
}

static void signal_handler(int sig, siginfo_t *si, void *unused)
{
	so_seg_t *segment = find_segment_of(si->si_addr);
	size_t length = (char *)si->si_addr - (char *)segment->vaddr;
	if (segment != NULL)
	{
		if (segment->data != NULL)
			exit(EXIT_FAILURE); // fault-ul este generat într-o pagină deja mapată, acces la memorie nepermis
		else
		{
			perror("Asta e cazul aici!");
			segment->data = mmap(si->si_addr, segment->mem_size - length, PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
			copy_into(segment, length);
			mprotect(segment->data, segment->mem_size - length, segment->perm);
		}
	}
	else
		exit(EXIT_FAILURE);
}

int so_init_loader(void)
{
	struct sigaction sig;

	memset(&sig, 0, sizeof(sig));
	sig.sa_flags = SA_SIGINFO;
	sigemptyset(&sig.sa_mask);
	sig.sa_sigaction = signal_handler;
	sigaction(SIGSEGV, &sig, NULL);

	return -1;
}

int so_execute(char *path, char *argv[])
{
	exec_decriptor = open(path, O_RDONLY); //if this fails then the so_parse_exec will exit
	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	return -1;
}
