#ifndef __H_TET_TETRIS_
#define __H_TET_TETRIS_

#include<gtk/gtk.h>
#include<glib.h>
#include"tet-shape.h"
#include"tet-canvas.h"



typedef struct _TetWin{
    TetCanvas* canvas;
    TetCanvas* preview;
    TetShape*shape;
    GtkWidget* info;
    
    GtkWidget* start,*stop,*pause;

    GtkWidget*window;

    gint score;
    gint time_start;
    gboolean overflowed;
    gboolean can_oper;

}TetWin;




TetWin* tet_window_new();
void tet_window_set_preview(TetWin*win,Shape type);
void tet_window_set_info(TetWin*win,char*text);
void tet_window_set_shape(TetWin*win,Shape type);
//void tet_window



#endif
