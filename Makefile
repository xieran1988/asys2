
plat ?= a8
libs += libxml-2.0
objs += algo.o teacher.o

include ${parentsdir}/top.mk

all: capfilter.so

%.o: algo.h

capfilter.so: ${objs}
	${CC} -shared -o $@ ${objs} ${LDFLAGS} 

clean:
	rm -rf *.o *.so 

