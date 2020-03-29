#include <Windows.h>
#include <stdio.h>

int x;

int main()
{
	MEMORY_BASIC_INFORMATION mbi;
	unsigned int ret;

	// make sure x is loaded into memory;
	x = 100;

	// query an address inside the code area
	ret = VirtualQuery(&main, &mbi, sizeof(mbi));
	if (!ret) {
		fprintf(stderr, "VirtualQuery failed\n");fflush(stderr);
		return 1;
	}

	// the permissions should be rx
	if (mbi.Protect != PAGE_EXECUTE_READ) {
		fprintf(stderr, "Code area doesn't have rx permissions\n");fflush(stderr);
		return 1;
	}

	// query an address inside the data area
	ret = VirtualQuery(&x, &mbi, sizeof(mbi));
	if (!ret) {
		fprintf(stderr, "VirtualQuery failed\n");fflush(stderr);
		return 1;
	}

	// the permissions should be rw
	if (mbi.Protect != PAGE_READWRITE) {
		fprintf(stderr, "Data area doesn't have rw permissions\n");fflush(stderr);
		return 1;
	}

	printf("SUCCESS\n");
	fflush(stdout);

	return 0;
}