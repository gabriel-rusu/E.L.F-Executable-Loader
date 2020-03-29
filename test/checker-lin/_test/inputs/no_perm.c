void exit(int code);

void _start()
{
	*(int *)0x44444444 = 0;
	exit(0);
}
