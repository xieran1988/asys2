#include <algo.h>

static char mode;
static char acting;
static xmlTextReaderPtr reader;
static xmlTextWriterPtr writer;

#define startnode(s) xmlTextWriterStartElement(writer, (xmlChar *)s);
#define endnode() xmlTextWriterEndElement(writer);
#define attr(s, ...) xmlTextWriterWriteFormatAttribute(writer, (xmlChar *)s, __VA_ARGS__)
#define nodename() ((char *)xmlTextReaderConstName(reader))
#define nodeis(s) !strcmp(nodename(), s)
#define nodeattr(s) (char *)xmlTextReaderGetAttribute(reader, (xmlChar *)s)
#define attris(s, v) (nodeattr(s) && !strcmp(nodeattr(s), v))
#define getattr(s, ...) (nodeattr(s) && sscanf(nodeattr(s), __VA_ARGS__))

void decl_radio(const char *name, int *sel, ...)
{
	va_list ap;
	int i;
	const char *o;

	va_start(ap, name);
	if (mode == 'w') {
		for (i = 0; o = va_arg(ap, const char *); i++) {
			startnode("radio"); 
			attr("name", "%s", name);
			attr("value", "%s", o);
			if (i == *sel) attr("checked", "%s", "");
			endnode();
		}
	}
	if (mode == 'r') {
		for (i = 0; o = va_arg(ap, const char *); i++) {
			if (nodeis("radio") && attris("name", name) && 
					attris("value", o) && attris("checked", "")) 
				*sel = i;
		}
	}
	va_end(ap);
}

void decl_btn(const char *name, void (*cb)(const char *))
{
	if (mode == 'w') {
		startnode("button");
		attr("name", "%s", name);
		endnode();
	}
	if (mode == 'r' && acting && 
			nodeis("button") && attris("name", name))
		cb(name);
}

void decl_var_int(const char *name, int *p, int min, int max)
{
	if (mode == 'w') {
		startnode("int");
		attr("name", "%s", name);
		attr("max", "%d", max);
		attr("min", "%d", min);
		attr("val", "%d", *p);
		endnode();
	}
	if (mode == 'r' && 
			nodeis("int") && attris("name", name)) 
	{
		int v;
		if (getattr("val", "%d", &v) && 
				v >= min && v <= max)
		 *p = v;	
	}
}

void decl_var_float(const char *name, float *p, float min, float max)
{
	if (mode == 'w') {
		startnode("float");
		attr("name", "%s", name);
		attr("max", "%f", max);
		attr("min", "%f", min);
		attr("val", "%f", *p);
		endnode();
	}
	if (mode == 'r' && 
			nodeis("float") && attris("name", name)) 
	{
		float v;
		if (getattr("val", "%f", &v) && 
				v >= min && v <= max) 
			*p = v;
	}
}

static void all_decl()
{
	decl();
}

static void readxml(const char *name)
{
	mode = 'r'; 
	reader = xmlReaderForFile(name, NULL, 0);
	xmlTextReaderRead(reader); // skip root
	while (xmlTextReaderRead(reader) == 1) {
		all_decl();
	}
	xmlFreeTextReader(reader);
}

static void writexml(const char *name)
{
	mode = 'w';
	writer = xmlNewTextWriterFilename(name, 0);
	xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
	startnode("root");
	all_decl();
	endnode();
	xmlFreeTextWriter(writer);
}

void fillrect(int x, int y, int w, int h, int val)
{
	while (h--) {
		memset(img[y] + x*2, val, w*2);
		y++;
	}
}

static int sock;

int selected(int fd)
{
	fd_set fs;
	struct timeval tv = {};
	FD_ZERO(&fs);
	FD_SET(fd, &fs);
	return select(fd + 1, &fs, NULL, NULL, &tv) > 0;
}

int udpsock(int port)
{
	struct sockaddr_in si;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	si.sin_family = AF_INET;
	si.sin_addr.s_addr = htonl(INADDR_ANY);
	si.sin_port = htons(port);
	bind(fd, (struct sockaddr *)&si, sizeof(si));
	return fd;
}

void udpsend(char *ip, int port, char *buf, int len)
{
	struct sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_addr.s_addr = inet_addr(ip);
	si.sin_port = htons(port);
	sendto(sock, buf, len, 0, (struct sockaddr *)&si, sizeof(si));
}

int udprecv(int fd, char *buf, int len)
{
	struct sockaddr_in sa;
	int salen;
	return recvfrom(fd, buf, len, 0, (struct sockaddr *)&sa, (socklen_t *)&salen);
}

void algo_init()
{
	printf("algo_init\n");
	sock = udpsock(1653);
	writexml("./default.xml");
	init();
}

void algo_fini()
{
	close(sock);
}

int algo_proc(void *_img)
{
	img = (typeof(img))_img;
	process();
	if (selected(sock)) {
		char buf[128];
		udprecv(sock, buf, sizeof(buf));
		if (*buf == 'q') 
			return 1;
		if (*buf == 'a') {
			acting = 1;
			readxml("./act.xml");
			acting = 0;
		}
		if (*buf == 's') 
			writexml("./config.xml");
		if (*buf == 'l')
			readxml("./config.xml");
		if (*buf == 'r') 
			readxml("./default.xml");
		writexml("./cur.xml");
	}
	return 0;
}

