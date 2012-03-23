#include <algo.h>

int sel;

void cb(const char *name)
{
	printf("btn pushed\n");
}

float f1;
int i1;

void decl()
{
	decl_radio("radio1", &sel, "item1", "item2", "item3", NULL);
	decl_btn("button1", cb);
	decl_var_float("f1", &f1, -30.123, 50.33);
	decl_var_int("i1", &i1, -123, 345);
}

void init()
{
	printf("init\n");
}

void process()
{
	static int n;
	printf("process %d: f1=%f i1=%d sel=%d\n", n, f1, i1, sel);
	fillrect((n*20)%(IMG_W-80), 300, 80, 80, 0xff);
	usleep(1000*300);
	n++;
}

