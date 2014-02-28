

OBJS=tet-tetwin.c tet-shape.c tet-canvas.c tet-tetris.c

tet-tetwin:${OBJS}
	gcc `pkg-config --cflags --libs gtk+-3.0` $^ -o $@


gcc `pkg-config --cflags --libs gtk+-3.0` test-tetwin.c tet-shape.c tet-canvas.c tet-tetris.c  -o test-tetwin
