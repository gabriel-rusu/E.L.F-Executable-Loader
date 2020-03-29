#include <Windows.h>

int arr_init[512] = { 0xdeadbeef };
int arr_uninit[16384];

void start()
{
	int i;
	DWORD bytesWritten;

	/* check initialized data */
	if (arr_init[0] != 0xdeadbeef) {
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "FAIL: initialized data\n", 23, NULL, NULL);
		ExitProcess(1);
	} else {
		/* check uninitialized data */
		for (i = 0; i < 16384; i++)
			if (arr_uninit[i] != 0) {
				WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "FAIL: bss\n", 10, NULL, NULL);
				ExitProcess(1);
			}
	}

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "SUCCESS\n", 8, &bytesWritten, NULL);

	ExitProcess(0);
}
