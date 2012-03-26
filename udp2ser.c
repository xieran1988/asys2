#include <algo.h>

int sel;

void decl()
{
	decl_radio("radio2", &sel, "cat", "dog", "bird", NULL);
}

int sock, ttyfd;

void init() 
{
	struct termios tio;

	sock = udpsock(1655);

	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	//tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	//tio.c_flag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	ttyfd = open("/dev/ttyS2", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio,B9600);            // 115200 baud
	cfsetispeed(&tio,B9600);            // 115200 baud
	tcsetattr(ttyfd, TCSANOW, &tio);
}

void process()
{
	static char buf[128*1024];
	int r;
	static int n;

	printf("process %d\n", n);
	n++;

	if (selected(sock)) {
		r = udprecv(sock, buf, sizeof(buf));
		write(ttyfd, buf, r);
	}
	if (selected(ttyfd)) {
		r = read(ttyfd, buf, sizeof(buf));
		udpreply(buf, r);
	}

	usleep(1000*300);
}

int main()
{
	algo_init();
	while (1) {
		if (algo_proc(NULL))
			return 0;
		usleep(1000*5);
	}
	return 0;
}

