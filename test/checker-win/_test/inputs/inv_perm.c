#include <Windows.h>

void start()
{
	// try to write in the code area. this should fault
	*(int *)&start = 0;
	ExitProcess(0);
}