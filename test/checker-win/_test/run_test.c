#include <Windows.h>
#include <stdio.h>
#include "loader.h"

#define HANDLER_CALL_FIRST	1

static int no_page_faults;
static char *fault_count_fname;

LONG CALLBACK test_segv_handler(PEXCEPTION_POINTERS pexc)
{
	FILE *f;
	errno_t ret;
	EXCEPTION_RECORD *er = pexc->ExceptionRecord;

	if (er->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	no_page_faults++;

	ret = fopen_s(&f, fault_count_fname, "w");
	if (!ret) {
		fprintf(f, "%d", no_page_faults);
		fclose(f);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (argc == 3)
		fault_count_fname = argv[2];
	else
		fault_count_fname = "fault_cnt.out";

	so_init_loader();

	if (AddVectoredExceptionHandler(HANDLER_CALL_FIRST,
					(PVECTORED_EXCEPTION_HANDLER)test_segv_handler) < 0) {
		fprintf(stderr, "cannot set signal handler\n");
		return 1;
	}

	so_execute(argv[1], argv+1);

	// Shouldn't reach here
	return 1;
}
