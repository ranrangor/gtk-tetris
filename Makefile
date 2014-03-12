

BINARY=tet-tetris

CC=gcc -O2 -std=c99
CFLAGS= `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`


OBJS=tet-tetris.o tet-shape.o tet-canvas.o tet-tetwin.o



.PHONY:clean




all:${OBJS}
	${CC} ${CFLAGS}  $^ -o ${BINARY} ${LIBS}


tet-shape.o:tet-shape.c tet-config.h
	${CC} ${CFLAGS} -c $< -o $@


tet-canvas.o:tet-canvas.c tet-config.h
	${CC} ${CFLAGS} -c $< -o $@

tet-tetwin.o:tet-tetwin.c tet-config.h
	${CC} ${CFLAGS} -c $< -o $@



clean:
	-rm *.o
	-rm ${BINARY}
