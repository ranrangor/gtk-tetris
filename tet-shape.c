#include"tet-shape.h"
//#include"tet-checker.h"


volatile static gboolean realize_locked = FALSE;


Point shape_path_O0[4] = { {0, 1}, {0, 2}, {-1, 1}, {-1, 2} };
Point shape_path_I0[4] = { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
Point shape_path_I1[4] = { {0, 1}, {-1, 1}, {-2, 1}, {-3, 1} };
Point shape_path_Z0[4] = { {-1, 0}, {-1, 1}, {0, 1}, {0, 2} };
Point shape_path_Z1[4] = { {0, 1}, {-1, 1}, {-1, 2}, {-2, 2} };
Point shape_path_L0[4] = { {0, 1}, {0, 2}, {-1, 1}, {-2, 1} };
Point shape_path_L1[4] = { {0, 1}, {-1, 1}, {-1, 2}, {-1, 3} };
Point shape_path_L2[4] = { {-2, 1}, {-2, 2}, {-1, 2}, {0, 2} };
Point shape_path_L3[4] = { {0, 1}, {0, 2}, {0, 3}, {-1, 3} };



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
	p = shape->lpath[i];
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


Shape get_shape_type()
{
    static GRand *random;
    random = g_rand_new();
    static Shape shapepool[] =
	{ TET_O0, TET_I0, TET_I1, TET_Z0, TET_Z1, TET_L0, TET_L1, TET_L2,
TET_L3 };

    return shapepool[g_rand_int(random) % 9];

}

TetShape *tet_shape_new(TetChecker * checker, int x, int y, Shape type)
{

    TetShape *shape = g_slice_new0(TetShape);

    shape->checker = checker;
    shape->x = shape->lx = x;
    shape->y = shape->ly = y;
    shape->type = type;



    switch (shape->type) {

    case TET_O0:{
	    shape_path_assign(shape, shape_path_O0);
	    shape->color = TET_COLOR_O;
	    break;
	}
    case TET_I0:{
	    shape_path_assign(shape, shape_path_I0);
	    shape->color = TET_COLOR_I;
	    break;
	}
    case TET_I1:{
	    shape_path_assign(shape, shape_path_I1);
	    shape->color = TET_COLOR_I;
	    break;
	}
    case TET_Z0:{
	    shape_path_assign(shape, shape_path_Z0);
	    shape->color = TET_COLOR_Z;
	    break;
	}
    case TET_Z1:{
	    shape_path_assign(shape, shape_path_Z1);
	    shape->color = TET_COLOR_Z;
	    break;
	}
    case TET_L0:{
	    shape_path_assign(shape, shape_path_L0);
	    shape->color = TET_COLOR_L;
	    break;
	}
    case TET_L1:{
	    shape_path_assign(shape, shape_path_L1);
	    shape->color = TET_COLOR_L;
	    break;
	}
    case TET_L2:{
	    shape_path_assign(shape, shape_path_L2);
	    shape->color = TET_COLOR_L;
	    break;
	}
    case TET_L3:{
	    shape_path_assign(shape, shape_path_L3);
	    shape->color = TET_COLOR_L;
	    break;
	}

    default:{
	    shape_path_assign(shape, shape_path_O0);
	    shape->color = TET_COLOR_O;
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


	if (x < 0) {

	    type |= COLLISION_TOP;
	    g_message("TOP[%d,%d]", x, y);

	}

	if (x >= shape->checker->height) {
	    type |= COLLISION_BOTTOM;
	    g_message("BOTTOM[%d,%d]", x, y);
	}
	/*
	   if (y>=CHECKER_WIDTH){//this case need to adjust to align border
	   type=COLLISION_SIDE;
	   break;
	   }

	 */
	if (is_filled(shape->checker, x, y)) {
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


    int max_y;			//the maximize x of the shape,may be ran out of the checker border

    int i;
    for ( i = 0; i < SHAPE_STEP; i++) {
	int y;
	y = shape->path[i].y + shape->y;
	if (y > max_y) {
	    max_y = y;
	}
    }
    int bias = max_y - CHECKER_WIDTH + 1;
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
	tet_checker_clear_block(shape->checker, x, y);
	tet_checker_fill(shape->checker, x, y, FALSE);
//      shape->checker->filling[x][y] = FALSE;
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

	tet_checker_clear_block(shape->checker, lx, ly);
//      shape->checker->filling[lx][ly] = FALSE;
    }

    for (i = 0; i < SHAPE_STEP; i++) {
	int x, y;
	x = shape->path[i].x + shape->x;
	y = shape->path[i].y + shape->y;
	
	tet_checker_color_block(shape->checker, x, y, shape->color);
	tet_checker_fill(shape->checker, x, y, TRUE);
    }
    realize_locked = FALSE;
    printf("\n[x:%d,y:%d]\n", shape->x, shape->y);

}

void tet_shape_move(TetShape * shape, int x, int y)
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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }

    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x = x;
    shape->y = y;

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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }


    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x -= 1;

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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }

    shape->ly = shape->y;
    shape->lx = shape->x;
    shape->x += 1;

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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }

    shape->lx = shape->x;
    shape->ly = shape->y;
    shape->y -= 1;		// shape->y+1;

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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }

    shape->lx = shape->x;
    shape->ly = shape->y;
    shape->y += 1;		// shape->y+1;

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

	tet_checker_fill(shape->checker, x, y, TRUE);
    }
    realize_locked = FALSE;

}




void tet_shape_transform_restore(TetShape* shape)
{

tet_shape_move_restore(shape);

//restore type
shape->type=shape->ltype;


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

	tet_checker_fill(shape->checker, x, y, FALSE);
    }
    shape->ltype=shape->type;




    int shapetype = shape->type;

    switch (shapetype & (~3)) {
	int subtype = shapetype & 3;

    case TET_O0:{
	    //donothing
	    break;
	}
    case TET_I0:{
	    if ((shapetype & 1) == 0)
		shape_path_assign(shape, shape_path_I1);
	    else
		shape_path_assign(shape, shape_path_I0);
	    shape->type = shapetype ^ 1;
//position adjusting when out of checker' border.

	    break;
	}
    case TET_Z0:{
	    if ((shapetype & 1) == 0)
		shape_path_assign(shape, shape_path_Z1);
	    else
		shape_path_assign(shape, shape_path_Z0);
	    shape->type = shapetype ^ 1;
	    break;
	}

    case TET_L0:{

	    if ((shapetype&3) == 0)
		shape_path_assign(shape, shape_path_L1);
	    else if ((shapetype&3) == 1)
		shape_path_assign(shape, shape_path_L2);
	    else if ((shapetype&3) == 2)
		shape_path_assign(shape, shape_path_L3);
	    else {
		shape_path_assign(shape, shape_path_L0);
	    }

	    shape->type = (shapetype&(~3) )| ((1+shapetype) & 3);
	    break;

	}

    default:{

	    g_warning("Something Error..!\n");

	}
    }
    shape->lx = shape->x;
    shape->ly = shape->y;

    realize_locked = FALSE;


}
