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
#include "exec_parser.h"

static so_exec_t *exec;

so_seg_t* find_segment_of(void *addr)
{
	for (int i = 0; i < exec->segments_no; i++)
		if ((char *)addr - (char *)exec->segments[i].vaddr < exec->segments[i].mem_size)
			return &(exec->segments[i]);
	return NULL;
}

static void signal_handler(int sig, siginfo_t *si, void *unused)
{
	so_seg_t *segment_addr = find_segment_of(si->si_addr);
	if (segment_addr !=NULL)
	{
		if(segment_addr->data!=NULL)
			exit(EXIT_FAILURE);	// fault-ul este generat într-o pagină deja mapată, acces la memorie nepermis
		else{
			printf("Asta e cazul aici!");
		}
	}
	else
		exit(EXIT_FAILURE);
}

int so_init_loader(void)
{
	/* TODO: initialize on-demand loader */
	struct sigaction sig;
	initialize(&mapped_pages);

	memset(&sig, 0, sizeof(sig));
	sig.sa_flags = SA_SIGINFO;
	sigemptyset(&sig.sa_mask);
	sig.sa_sigaction = signal_handler;
	sigaction(SIGSEGV, &sig, NULL);

	return -1;
}

int so_execute(char *path, char *argv[])
{
	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	return -1;
}
