void exit(int code);

void _start()
{
	*(int *)&_start = 0;
	exit(0);
}
