#ifndef __H_SHAPE_TET_
#define __H_SHAPE_TET_


#include<gtk/gtk.h>
#include"tet-canvas.h"
#include"tet-config.h"



typedef enum _CollisionType {
    COLLISION_NONE = 0,
    COLLISION_FILL = 1,
    COLLISION_BOTTOM = 2
//,COLLISION_SIDE=4
} CollisionType;



typedef enum _Shape {		/*
				 *    ##
				 *    ##
				 *
				 * */
    TET_O0 = 4,

    /*    #
     *    #
     *    #
     *    #
     *
     * */
    TET_I0 = 8, TET_I1,

    /*   #
     *   ##
     *    #
     *
     * */
    TET_Z0 = 16, TET_Z1,



    /*    
     *    #
     *    #
     *    ##
     *
     * */
//    TET_L0=32,TET_L1,TET_L2,TET_L3,

    /*
     *    #
     *   ###
     *
     * */
//    TET_T0=64,TET_T1,TET_T2,TET_T3,
    TET_LAST
} Shape;

typedef struct _point {
    int x;
    int y;
} Point;



typedef struct _TetShape {
    Point path[4];
    Point lpath[4];
    TetCanvas *canvas;
    Shape type;
//coordination about left-bottom of shape
    gint x;
    gint y;
    gint lx;
    gint ly;
} TetShape;




#define FOR_PATH(shape,x,y) int i;\
    for(i=0;i<SHAPE_STEP;i++){\
        x=shape->path[i].x+shape->x;\
        y=shape->path[i].y+shape->y;

#define FOR_PATH_END }



void shape_path_assign(TetShape * shape, Point * path);
void shape_path_save(TetShape * shape);
void shape_path_swap(TetShape * shape);
//static void shape_path_fill()

TetShape *tet_shape_new(TetCanvas * canvas, Shape type);
void tet_shape_free(TetShape * shape);

void tet_shape_transform(TetShape * shape);
static void tet_shape_align_border(TetShape * ahape);	//invoked by transform only;
CollisionType tet_shape_is_collision(TetShape * shape);

void tet_shape_move_up(TetShape * shape);
void tet_shape_move_down(TetShape * shape);
void tet_shape_move_left(TetShape * shape);
void tet_shape_move_right(TetShape * shape);
void tet_shape_move_restore(TetShape * shape);


void tet_shape_realize(TetShape * shape);
//TetShape* tet_shape_save(TetShape * shape);
//void tet_shape_clear(TetShape * shape);
#endif
