/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "exec_parser.h"


static so_exec_t *exec;
typedef struct mapped_pages{
	void **pages;
	int no_pages;
}loader;

initialize(loader **mapped_pages){
	*mapped_pages = malloc(sizeof(loader));
	(*mapped_pages)->pages = NULL;
	(*mapped_pages)->no_pages = 0;
}

static void signal_handler(int sig, siginfo_t *si, void *unused)
{
	printf("Am ajuns in %s\n",__FUNCTION__);
	printf("Got SIGSEGV at address: 0x%lx\n",(long) si->si_addr);

	printf("Base addres for executable: %p\n",exec->base_addr);
	printf("Adresele parsate sunt:\n");
	for(int i =0;i <exec->segments_no;i++)
		printf("-> %p\n",(void*)exec->segments[i].vaddr);
}

int so_init_loader(void)
{
	/* TODO: initialize on-demand loader */
	struct sigaction sig;
	loader *mapped_pages = NULL;
	initialize(&mapped_pages);
 
    memset(&sig, 0, sizeof(sig));
	sig.sa_handler = signal_handler;
	sigaction(SIGSEGV,&sig,NULL);

	return -1;
}

int so_execute(char *path, char *argv[])
{
	exec = so_parse_exec(path);
	printf("Adresele parsate sunt: ");
	for(int i =0;i <exec->segments_no;i++)
		printf("%p\n",(void*)exec->segments->vaddr);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	return -1;
}
