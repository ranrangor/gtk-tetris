#ifndef __H_CHECKER_TET_
#define __H_CHECKER_TET_


#include<gtk/gtk.h>
#include"tet-config.h"



typedef struct _checker {
    gint width;
    gint height;
//    GtkWidget *grid[CANVAS_HEIGHT][CANVAS_WIDTH];
    GtkWidget**grid;
    GtkWidget *container;

//    gboolean filling[CANVAS_HEIGHT][CANVAS_WIDTH];
    gboolean *filling;
} TetChecker;



TetChecker *tet_checker_new(int height,int width,int block_siz);
void tet_checker_free(TetChecker * checker);

void tet_checker_color_all(TetChecker * checker, char *rgba);
void tet_canvas_clear_all(TetChecker * checker);

void tet_checker_color_block(TetChecker * can, gint i, gint j, char *rgba);
void tet_checker_clear_block(TetChecker * can, gint i, gint j);

//gboolean tet_checker_copy_block(TetChecker*checker,int i,int j,int ii,int jj);
//gboolean tet_checker_copy_fill(TetChecker*checker,int i,int j,int ii,int jj);

int tet_checker_eliminate(TetChecker*checker,int baseline);



//inline 
void tet_checker_fill(TetChecker * checker, int i, int j,
			    gboolean fill);
//inline 
void tet_checker_fill_all(TetChecker * checker,gboolean fill);

void tet_checker_free(TetChecker * checker);


//inline 
gboolean is_filled(TetChecker * checker, gint i, gint j);






#endif
