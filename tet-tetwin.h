#ifndef __H_TET_WIN_
#define __H_TET_WIN_

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
    gint timeout;
    gint key_sig_no;
    gboolean paused;

}TetWin;




TetWin* tet_window_new();
void tet_window_set_preview(TetWin*win,Shape type);
//void tet_window_update(TetWin*win,int l);
void tet_window_set_info(TetWin*win,char*text);
void tet_window_set_shape(TetWin*win,TetShape*shape);
//void tet_window

void tet_window_reset(TetWin*win);


#endif
