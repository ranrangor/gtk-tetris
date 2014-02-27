#include<gtk/gtk.h>
#include<glib.h>





TetWin* tet_window_new()
{

    TetWin*tetwin=g_slice_new0(TetWin);

    tetwin->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    tetwin->preview=tet_canvas_


}




void tet_window_set_preview(TetWin*win,Shape type);
void tet_window_set_info(TetWin*win,char*text);
