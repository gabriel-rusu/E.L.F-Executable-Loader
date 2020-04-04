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

void signal_handler(int signum)
{
	
}

int so_init_loader(void)
{
	/* TODO: initialize on-demand loader */
	struct sigaction sig;
 
    memset(&sig, 0, sizeof(sig));
	sig.sa_handler = signal_handler
	sigaction(SIGSEGV,&sig,NULL)

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
