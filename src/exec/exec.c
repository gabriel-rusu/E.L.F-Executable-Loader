/*
 * Operating System Executable Loader Exec tester
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include "loader.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Please provide an executable!\n");
		return 1;
	}

	so_init_loader();
	so_execute(argv[1], argv + 1);

	return 0;
}
