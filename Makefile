
plat ?= a8
libs += libxml-2.0
teacher_objs += algo.o teacher.o
udp2ser_objs += algo.o udp2ser.o
release_files += teacher.so udp2ser

include ${parentsdir}/top.mk

all: teacher.so udp2ser

%.o: algo.h

udp2ser: ${udp2ser_objs}
	${CC} -o $@ ${udp2ser_objs} ${LDFLAGS}

teacher.so: ${teacher_objs}
	${CC} -shared -o $@ ${teacher_objs} ${LDFLAGS} 

clean:
	rm -rf *.o *.so 

