#include <Windows.h>

#define ARR_SIZE 16384

int arr[ARR_SIZE];

void start()
{
	int i;
	char fail_str[5];
	char success_str[8];
	DWORD bytesWritten;

	//force these strings on the stack
	fail_str[0] = 'F';
	fail_str[1] = 'A';
	fail_str[2] = 'I';
	fail_str[3] = 'L';
	fail_str[4] = '\n';

	success_str[0] = 'S';
	success_str[1] = 'U';
	success_str[2] = 'C';
	success_str[3] = 'C';
	success_str[4] = 'E';
	success_str[5] = 'S';
	success_str[6] = 'S';
	success_str[7] = '\n';

	//check that every value in the array is 0
	for (i = 0; i < ARR_SIZE; i++)
		if (arr[i] != 0) {
			WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), fail_str, 5, NULL, NULL);
			ExitProcess(1);
		}

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), success_str, 8, &bytesWritten, NULL);

	ExitProcess(0);
}
