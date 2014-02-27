

OBJS=tet-main.c tet-shape.c tet-canvas.c

tet-main:${OBJS}
	gcc `pkg-config --cflags --libs gtk+-3.0` $^ -o $@
