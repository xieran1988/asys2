#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <syslog.h>
#include <dirent.h>
#include <termios.h>
#include <fcntl.h>
#include <math.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <libxml/encoding.h>
#include <jpeglib.h>

#define IMG_W 720
#define IMG_H 576

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

unsigned char (*img)[IMG_W*2];
void fillrect(int x, int y, int w, int h, int val);

void udpsend(char *ip, int port, char *buf, int len);

void decl_radio(const char *name, int *sel, ...);
void decl_btn(const char *name, void (*cb)(const char *));
void decl_var_float(const char *name, float *p, float min, float max);
void decl_var_int(const char *name, int *p, int min, int max);

void init();
void decl();
void process();

