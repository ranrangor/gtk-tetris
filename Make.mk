

OBJS=tet-tetwin.c tet-shape.c tet-canvas.c tet-tetris.c

tet-tetwin:${OBJS}
	gcc `pkg-config --cflags --libs gtk+-3.0` $^ -o $@
