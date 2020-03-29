#include <Windows.h>

void start()
{
	// access an invalid address. this should fault
	*(int *)0x44444444 = 0;
	ExitProcess(0);
}