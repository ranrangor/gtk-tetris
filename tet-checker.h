#ifndef __H_CHECKER_TET_
#define __H_CHECKER_TET_


#include<gtk/gtk.h>
#include"tet-config.h"



typedef struct _checker {
    gint width;
    gint height;
    GtkWidget**grid;
    GtkWidget *container;
    gint bsiz;
    gboolean *filling;

} TetChecker;




TetChecker *tet_checker_new(int height,int width,int block_siz);
void tet_checker_free(TetChecker * checker);

void tet_checker_color_all(TetChecker * checker, char *rgba);
void tet_checker_clear_all(TetChecker * checker);

void tet_checker_color_block(TetChecker * can, gint i, gint j, char *rgba);
void tet_checker_clear_block(TetChecker * can, gint i, gint j);


int tet_checker_eliminate(TetChecker*checker,int baseline);



void tet_checker_fill(TetChecker * checker, int i, int j,
			    gboolean fill);
void tet_checker_fill_all(TetChecker * checker,gboolean fill);

void tet_checker_free(TetChecker * checker);

gboolean is_filled(TetChecker * checker, gint i, gint j);






#endif
