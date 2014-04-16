#include<gtk/gtk.h>
#include<glib.h>
#include"tet-tetwin.h"
#include"tet-config.h"


static const char* fmt="<span font_family=\"Serif\" font_weight=\"bold\" >:Info:\n</span><span font_family=\"Sans\">Score:\%d</span>\n";


TetWin* tet_window_new()
{

    TetWin*tetwin=g_slice_new0(TetWin);

    tetwin->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable(GTK_WINDOW(tetwin->window),FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(tetwin->window),5);

    tetwin->preview=tet_checker_new(PREVIEW_HEIGHT,PREVIEW_WIDTH,PREVIEW_BSIZ);
    tetwin->checker=tet_checker_new(CHECKER_HEIGHT,CHECKER_WIDTH,CHECKER_BSIZ);

  tet_checker_clear_all (tetwin->checker);
  tet_checker_clear_all (tetwin->preview);

    tetwin->info=gtk_label_new("");
    gchar*mark=g_markup_printf_escaped(fmt,0);
    gtk_label_set_markup(GTK_LABEL(tetwin->info),mark);
    gtk_label_set_justify(GTK_LABEL(tetwin->info),GTK_JUSTIFY_CENTER);
    g_free(mark);
    
    tetwin->start=gtk_button_new_with_label("Start");
    tetwin->stop=gtk_button_new_with_label("Stop");
    tetwin->pause=gtk_button_new_with_label("Pause");
    gtk_widget_set_sensitive(tetwin->stop,FALSE);
    gtk_widget_set_sensitive(tetwin->pause,FALSE);

    GtkWidget*box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    gtk_box_pack_start(GTK_BOX(box),tetwin->checker->container,FALSE,FALSE,5);

    GtkWidget*panelbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_box_pack_start(GTK_BOX(panelbox),tetwin->preview->container,FALSE,FALSE,5);

    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->stop,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->pause,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->start,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(panelbox),tetwin->info,FALSE,FALSE,0);

    gtk_box_pack_end(GTK_BOX(box),panelbox,FALSE,FALSE,5);
    gtk_container_add(GTK_CONTAINER(tetwin->window),box);


    g_signal_connect(G_OBJECT(tetwin->window),"delete-event",G_CALLBACK(gtk_main_quit),NULL);

    return tetwin;


}


void tet_window_over(TetWin*tetwin)
{

    g_source_remove(tetwin->timeout);
    g_signal_handler_disconnect(tetwin->window,tetwin->key_sig_no);

    gtk_widget_grab_focus(tetwin->window);

    GtkWidget*msgdialog=gtk_message_dialog_new(GTK_WINDOW(tetwin->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Your score is %d. ^.^",tetwin->score);

    if(GTK_RESPONSE_CLOSE==gtk_dialog_run(GTK_DIALOG(msgdialog)))
    {
    
    g_message("Closed.");
    tet_window_reset(tetwin);
  tet_checker_clear_all (tetwin->checker);
  tet_checker_clear_all (tetwin->preview);
    tet_checker_fill_all(tetwin->checker,FALSE);
    
    }

    gtk_widget_destroy(msgdialog);
    gtk_widget_set_sensitive(tetwin->start,TRUE);
    gtk_widget_set_sensitive(tetwin->pause,FALSE);
    gtk_widget_set_sensitive(tetwin->stop,FALSE);

}











void tet_window_reset(TetWin*win)
{

    win->score=0;
    win->time_start=0;
tet_window_set_info(win,"");


}


void tet_window_set_preview(TetWin*win,Shape type)
{

    /*Clean every shape at win->preview*/
    tet_checker_clear_all(win->preview);
    tet_checker_fill_all(win->preview,FALSE);

    /*paint a new shape with type*/
    TetShape*shape=tet_shape_new(win->preview,win->preview->height-1,0,type);
//    tet_shape_move(shape,...);//
//    shape_print(shape,"set preview");
    tet_shape_realize(shape);
    
    tet_shape_free(shape);


}


void tet_window_set_shape(TetWin*win,TetShape *shape)
{
    win->shape=shape;

}



void tet_window_set_info(TetWin*win,char*text)
{

    gchar*mark=g_markup_printf_escaped(fmt,win->score);
    gtk_label_set_markup(GTK_LABEL(win->info),mark);
    gtk_label_set_justify(GTK_LABEL(win->info),GTK_JUSTIFY_CENTER);
    g_free(mark);

}




