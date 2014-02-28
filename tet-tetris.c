#include<gtk/gtk.h>
#include<glib.h>
#include"tet-tetris.h"




TetWin* tet_window_new()
{

    TetWin*tetwin=g_slice_new0(TetWin);

    tetwin->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    tetwin->preview=tet_canvas_new(PREVIEW_HEIGHT,PREVIEW_WIDTH,PREVIEW_BSIZ);
    tetwin->canvas=tet_canvas_new(CANVAS_HEIGHT,CANVAS_WIDTH,CANVAS_BSIZ);
    tetwin->info=gtk_label_new("===Info===");
    
    tetwin->start=gtk_button_new_with_label("Start");
    tetwin->stop=gtk_button_new_with_label("Stop");
    tetwin->pause=gtk_button_new_with_label("Pause");

    GtkWidget*box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    gtk_box_pack_start(GTK_BOX(box),tetwin->canvas->container,FALSE,FALSE,0);

    GtkWidget*panelbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_box_pack_start(GTK_BOX(panelbox),tetwin->preview->container,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(panelbox),tetwin->info,TRUE,TRUE,0);

    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->stop,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->pause,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->start,FALSE,FALSE,0);

    gtk_box_pack_end(GTK_BOX(box),panelbox,FALSE,FALSE,0);
    gtk_container_add(GTK_CONTAINER(tetwin->window),box);


    return tetwin;


}




void tet_window_set_preview(TetWin*win,Shape type)
{

    /*Clean every shape at win->preview*/
    tet_canvas_clear_all(win->preview);
    tet_canvas_fill_all(win->preview,FALSE);

    /*paint a new shape with type*/
    TetShape*shape=tet_shape_new(win->preview,win->preview->height-1,0,type);
//    tet_shape_move(shape,...);//
    shape_print(win->preview,"set preview");
    tet_shape_realize(shape);
    
    tet_shape_free(shape);


}


void tet_window_set_shape(TetWin*win,Shape type)
{

    TetShape*shape=tet_shape_new(win->canvas,0,0,type);

//    tet_shape_realize(shape);


}






void tet_window_set_info(TetWin*win,char*text)
{
    char*info=g_strdup_printf("Score:\t %d\n%s",win->score,text);

    gtk_label_set_text(GTK_LABEL(win->info),info);
    g_free(info);

}




