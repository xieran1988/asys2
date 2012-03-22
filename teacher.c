#include <algo.h>

void decl()
{
}

void init()
{
	printf("init\n");
}

void process()
{
	static int n;
	printf("process %d\n", n);
	fillrect((n*20)%(IMG_W-80), 300, 80, 80, 0xff);
	usleep(1000*300);
	n++;
}

