#include"tet-shape.h"
//#include"tet-canvas.h"


volatile static gboolean realize_locked = FALSE;


Point shape_path_O0[4] = { {0, 0}, {0, 1}, {-1, 0}, {-1, 1} };
Point shape_path_I0[4] = { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
Point shape_path_I1[4] = { {0, 0}, {-1, 0}, {-2, 0}, {-3, 0} };
Point shape_path_Z0[4] = { {-1, 0}, {-1, 1}, {0, 1}, {0, 2} };
Point shape_path_Z1[4] = { {0, 0}, {-1, 0}, {-1, 1}, {-2, 1} };



void shape_print(TetShape * shape, char *text)
{

    g_print("%s\n", text);
    g_print("[x:%d,y:%d]<--[lx:%d,ly:%d]\n", shape->x, shape->y, shape->lx,
	    shape->ly);
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	g_print("[%d,%d].", shape->path[i].x, shape->path[i].y);
    }
    g_print("\n");


}

void shape_path_swap(TetShape * shape)
{
    int i;
    Point p;
    for (i = 0; i < SHAPE_STEP; i++) {
        p=shape->lpath[i];
	    shape->lpath[i] = shape->path[i];
        shape->path[i] = p;
    }

}

void shape_path_save(TetShape * shape)
{
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	shape->lpath[i] = shape->path[i];

    }


}

void shape_path_assign(TetShape * shape, Point * path)
{

    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	shape->path[i] = path[i];

    }
}



TetShape *tet_shape_new(TetCanvas * canvas,int x,int y,Shape type)
{

    TetShape *shape = g_slice_new0(TetShape);

    shape->canvas = canvas;
    shape->x = shape->lx = x;
    shape->y = shape->ly = y;
    shape->type = type;



    switch (shape->type) {

    case TET_O0:{
//          shape->path = shape_path_O0;
	    shape_path_assign(shape, shape_path_O0);
	    break;
	}
    case TET_I0:{
//          shape->path = shape_path_I0;
	    shape_path_assign(shape, shape_path_I0);
	    break;
	}
    case TET_I1:{
//          shape->path = shape_path_I1;
	    shape_path_assign(shape, shape_path_I1);
	    break;
	}
    case TET_Z0:{
//          shape->path = shape_path_Z0;
	    shape_path_assign(shape, shape_path_Z0);
	    break;
	}
    case TET_Z1:{
//          shape->path = shape_path_Z1;
	    shape_path_assign(shape, shape_path_Z1);
	    break;
	}
    default:{
//          shape->path = shape_path_O0;
	    shape_path_assign(shape, shape_path_O0);
	}

    }

    return shape;

}

void tet_shape_free(TetShape * shape)
{
    g_slice_free(TetShape, shape);
}

CollisionType tet_shape_is_collision(TetShape * shape)
{
    CollisionType type = COLLISION_NONE;	//not collision;
    int i;
    while (realize_locked);
    realize_locked = TRUE;
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;

	if (x >= CANVAS_HEIGHT) {
	    type |= COLLISION_BOTTOM;
	    g_message("BOTTOM[%d,%d]", x, y);
//        break;
	}
	/*
	   if (y>=CANVAS_WIDTH){//this case need to adjust to align border
	   type=COLLISION_SIDE;
	   break;
	   }

	 */
	if (is_filled(shape->canvas, x, y)) {
	    type |= COLLISION_FILL;
	    g_message("FILLED[%d,%d]", x, y);
//          break;
	}
    }
    realize_locked = FALSE;
    return type;


}

/*
void tet_shape_align_border(TetShape * shape)
{


    int max_y;			//the maximize x of the shape,may be ran out of the canvas border

    int i;
    for ( i = 0; i < SHAPE_STEP; i++) {
	int y;
	y = shape->path[i].y + shape->y;
	if (y > max_y) {
	    max_y = y;
	}
    }
    int bias = max_y - CANVAS_WIDTH + 1;
    if (bias > 0)
	shape->y -= bias;

}


*/


void tet_shape_clear(TetShape * shape)
{
    int i;
//    while(realize_locked);

    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
	tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,FALSE);
//	shape->canvas->filling[x][y] = FALSE;
    }

}

void tet_shape_realize(TetShape * shape)
{
    int i;
    while (realize_locked);
    realize_locked = TRUE;
    for (i = 0; i < SHAPE_STEP; i++) {
	int lx, ly;
	lx = shape->lpath[i].x + shape->lx;
	ly = shape->lpath[i].y + shape->ly;
//    if(lx>=0 && ly>=0 && ly<CANVAS_WIDTH){
	tet_canvas_clear_block(shape->canvas, lx, ly);
//      shape->canvas->filling[lx][ly] = FALSE;
//    }
    }

    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
	g_print("[%d,%d]\n", x, y);
//    if(x>=0 && y>=0 && y<CANVAS_WIDTH){
	tet_canvas_color_block(shape->canvas, x, y, SHAPE_COLOR);
    tet_canvas_fill(shape->canvas,x,y,TRUE);
//    }
    }
    realize_locked = FALSE;
    printf("\n[x:%d,y:%d]\n", shape->x, shape->y);

}

void tet_shape_move(TetShape*shape,int x,int y)
{

    int i;
    while (realize_locked);
    realize_locked = TRUE;

    shape_path_save(shape);
    //copy path to lpath

    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,FALSE);
//	shape->canvas->filling[x][y] = FALSE;
    }

//    printf("\n[x:%d,y:%d]-->",shape->x,shape->y);
    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x = x;
    shape->y = y;
//    printf("[x:%d,y:%d]\n",shape->x,shape->y);

    realize_locked = FALSE;

}

void tet_shape_move_up(TetShape * shape)
{

    int i;
    while (realize_locked);
    realize_locked = TRUE;

    shape_path_save(shape);
    //copy path to lpath

    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,FALSE);
//	shape->canvas->filling[x][y] = FALSE;
    }


//    printf("\n[x:%d,y:%d]-->",shape->x,shape->y);
    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x -= 1;
//    printf("[x:%d,y:%d]\n",shape->x,shape->y);

    realize_locked = FALSE;
}



void tet_shape_move_down(TetShape * shape)
{

    int i;
    while (realize_locked);
    realize_locked = TRUE;
    shape_path_save(shape);
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,FALSE);
	//shape->canvas->filling[x][y] = FALSE;
    }


//    printf("\n[x:%d,y:%d]-->",shape->x,shape->y);
    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x += 1;
//    printf("[x:%d,y:%d]\n",shape->x,shape->y);

    realize_locked = FALSE;
}

void tet_shape_move_left(TetShape * shape)
{
    while (realize_locked);
    realize_locked = TRUE;
    shape_path_save(shape);
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,FALSE);
//	shape->canvas->filling[x][y] = FALSE;
    }


//    printf("\n[x:%d,y:%d]-->",shape->x,shape->y);
    shape->lx = shape->x;
    shape->ly = shape->y;
    shape->y -= 1;		// shape->y+1;
//    printf("[x:%d,y:%d]\n",shape->x,shape->y);
    realize_locked = FALSE;
}

void tet_shape_move_right(TetShape * shape)
{
    while (realize_locked);
    realize_locked = TRUE;
    shape_path_save(shape);
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
//	shape->canvas->filling[x][y] = FALSE;
    tet_canvas_fill(shape->canvas,x,y,FALSE);
    }



//    printf("\n[x:%d,y:%d]-->",shape->x,shape->y);
    shape->lx = shape->x;
    shape->ly = shape->y;
    shape->y += 1;		// shape->y+1;
//    printf("[x:%d,y:%d]\n",shape->x,shape->y);
    realize_locked = FALSE;
}


void tet_shape_move_restore(TetShape * shape)
{
    while (realize_locked);
    realize_locked = TRUE;
    shape->x = shape->lx;
    shape->y = shape->ly;
    shape_path_swap(shape);
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
    tet_canvas_fill(shape->canvas,x,y,TRUE);
//	shape->canvas->filling[x][y] = TRUE;
    }
    realize_locked = FALSE;

}


void tet_shape_transform(TetShape * shape)
{
    while (realize_locked);
    realize_locked = TRUE;
    shape_path_save(shape);
    int i;
    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->lpath[i].x + shape->x;
	y = shape->lpath[i].y + shape->y;
//      tet_canvas_clear_block(shape->canvas, x, y);
//	shape->canvas->filling[x][y] = FALSE;
    tet_canvas_fill(shape->canvas,x,y,FALSE);
    }




    int shapetype = shape->type;

    switch (shapetype & (~3)) {
	int subtype = shapetype & 3;

    case TET_O0:{
	    //donothing
	    break;
	}
    case TET_I0:{
	    if ((shapetype & 1) == 0)
//              shape->path = shape_path_I1;
		shape_path_assign(shape, shape_path_I1);
	    else
		shape_path_assign(shape, shape_path_I0);
//              shape->path = shape_path_I0;
	    shape->type = shapetype ^ 1;
//position adjusting when out of canvas' border.

	    break;
	}
    case TET_Z0:{
	    if ((shapetype & 1) == 0)
		shape_path_assign(shape, shape_path_Z1);
//              shape->path = shape_path_Z1;
	    else
		shape_path_assign(shape, shape_path_Z0);
//              shape->path = shape_path_Z0;
	    shape->type = shapetype ^ 1;
	    break;
	}
/*
    case TET_L0:{
                
        if(shapetype&3==0)
            shape->path=shape_path_Z1;
        else
            shape->path=shape_path_Z0;
        break;
                
                
                
                }

*/
    default:{

	    printf("Something Error..!\n");

	}
    }
    shape->lx=shape->x;
    shape->ly=shape->y;

    realize_locked=FALSE;
/*
    CollisionType collision_type = COLLISION_NONE;
    collision_type = tet_shape_is_collision(shape);
    if (collision_type == COLLISION_NONE) {
	//not got collision
	tet_shape_realize(shape);
    } else if (collision_type == COLLISION_FILL) {
	//got collision
	//do nothing 
    } else if (collision_type == COLLISION_BOTTOM) {
	//to bottom of canvas
	//do nothing 
    } else {			//collision_type==COLLISION_SIDE;
	//do nothing 

    }

*/
}
