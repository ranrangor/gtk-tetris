#ifndef __H_CANVAS_TET_
#define __H_CANVAS_TET_


#include<gtk/gtk.h>
#include"tet-config.h"



typedef struct _canvas {
    gint width;
    gint height;
//    GtkWidget *grid[CANVAS_HEIGHT][CANVAS_WIDTH];
    GtkWidget**grid;
    GtkWidget *container;

//    gboolean filling[CANVAS_HEIGHT][CANVAS_WIDTH];
    gboolean *filling;
} TetCanvas;



TetCanvas *tet_canvas_new(int height,int width,int block_siz);
void tet_canvas_free(TetCanvas * canvas);

void tet_canvas_color_all(TetCanvas * canvas, char *rgba);
void tet_canvas_clear_all(TetCanvas * canvas);

void tet_canvas_color_block(TetCanvas * can, gint i, gint j, char *rgba);
void tet_canvas_clear_block(TetCanvas * can, gint i, gint j);

//gboolean tet_canvas_copy_block(TetCanvas*canvas,int i,int j,int ii,int jj);
//gboolean tet_canvas_copy_fill(TetCanvas*canvas,int i,int j,int ii,int jj);

int tet_canvas_eliminate(TetCanvas*canvas,int baseline);



//inline 
void tet_canvas_fill(TetCanvas * canvas, int i, int j,
			    gboolean fill);
//inline 
void tet_canvas_fill_all(TetCanvas * canvas,gboolean fill);

void tet_canvas_free(TetCanvas * canvas);


//inline 
gboolean is_filled(TetCanvas * canvas, gint i, gint j);






#endif
