#include<gtk/gtk.h>
#include<glib.h>

#include"tet-canvas.h"
#include"tet-shape.h"
#include"tet-tetris.h"

  TetShape *curshape, *nextshape;


  GRand *random;

  gboolean timeout_act(TetWin*win)
{
    int n_eliminated;
    CollisionType col;
//    TetShape*shape=win->shape;
    TetCanvas*canvas=win->canvas;
    TetCanvas*preview=win->preview;

    /*fall*/
    tet_shape_move_down(win->shape);


    col=tet_shape_is_collision(win->shape);
   
    if(COLLISION_FILL &col){

    //can not fall anymore;
    tet_shape_move_restore(win->shape);
    n_eliminated=tet_canvas_eliminate(canvas,win->shape->x);
   win->score+=n_eliminated; 

    tet_window_set_info(win,".");
    tet_shape_free(win->shape);
    
    col=tet_shape_is_collision(win->shape);
/*    if(COLLISION_TOP&col){
        g_message("Game Over @1");
        return FALSE;  
    }
*/
    /*get next shape*/
//    win->shape=nextshape;
    tet_window_set_shape(win,nextshape);
//    gint tet_type=g_rand_int(random)%5;

    Shape next=get_shape_type();
    nextshape=tet_shape_new(canvas,0,3,next);
    tet_window_set_preview(win,next);

    col=tet_shape_is_collision(win->shape);
    if(COLLISION_FILL&col){
        g_message("GameOver..");
        return FALSE;
    }
    tet_shape_realize(win->shape);
    /**/
    return TRUE;

    }
    
    else{
    tet_shape_realize(win->shape);
    
    return TRUE;
    } 

}


gboolean move_cb(GtkWidget * widget, GdkEventKey * event, TetWin*win)
{
TetShape * shape=win->shape;
    if (event->type == GDK_KEY_RELEASE) {
	if (event->keyval == GDK_KEY_Left) {
	    tet_shape_move_left(shape);
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		tet_shape_move_restore(shape);
	    } else {
		tet_shape_realize(shape);
	    }

	} else if (event->keyval == GDK_KEY_Right) {

	    tet_shape_move_right(shape);
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		tet_shape_move_restore(shape);
	    } else {
		tet_shape_realize(shape);
	    }
	} else if (event->keyval == GDK_KEY_Down) {

	    tet_shape_move_down(shape);
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		tet_shape_move_restore(shape);
	    } else {
		tet_shape_realize(shape);
	    }

	}
else if(event->keyval==GDK_KEY_Up){

tet_shape_transform(shape);
if(tet_shape_is_collision(shape)&COLLISION_FILL){
    tet_shape_move_restore(shape);
}else{

tet_shape_realize(shape);

}

}

    }

    return TRUE;

}










void
stop_cb (GtkWidget * widget, TetWin * tetwin)
{

    g_source_remove(tetwin->timeout);
    g_signal_handler_disconnect(tetwin->window,tetwin->key_sig_no);

//  tet_canvas_clear_all (tetwin->canvas);
//  tet_canvas_clear_all (tetwin->preview);
//    tet_canvas_fill_all(tetwin->canvas,FALSE);
    
//    tet_window_reset(tetwin);


//    gchar*info=g_strdup_printf("Your score is %d. ^.^",tetwin->score);

    GtkWidget*msgdialog=gtk_message_dialog_new(GTK_WINDOW(tetwin->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Your score is %d. ^.^",tetwin->score);
 //   g_free(info);



}


void
pause_cb (GtkWidget * widget, TetWin * tetwin)
{
    if(tetwin->paused){//restart
        tetwin->paused=FALSE;
    g_signal_handler_unblock(tetwin->window,tetwin->key_sig_no);
    tetwin->timeout=g_timeout_add(1000,(GSourceFunc)timeout_act,tetwin);
    }
    else{
    
        tetwin->paused=TRUE;
    g_signal_handler_block(tetwin->window,tetwin->key_sig_no);
    g_source_remove(tetwin->timeout);
    
    
    }

}




void
start_cb (GtkWidget * widget, TetWin * tetwin)
{

//  nextshape = tet_shape_new (win->canvas, 0, 0, TET_Z0);
//  win->can_oper = TRUE;
//  tet_window_set_preview (win, TET_Z0);

    tet_window_reset(tetwin);
  tet_canvas_clear_all (tetwin->canvas);
  tet_canvas_clear_all (tetwin->preview);
    tet_canvas_fill_all(tetwin->canvas,FALSE);


    Shape cur,next;
    next=get_shape_type();
    tet_window_set_preview (tetwin, next);
    nextshape = tet_shape_new (tetwin->canvas, 0, CANVAS_MID, next);
  

    cur=get_shape_type();
    curshape = tet_shape_new (tetwin->canvas, -1, CANVAS_MID, cur);
    tetwin->shape=curshape;
    


    tetwin->timeout=g_timeout_add(1000,(GSourceFunc)timeout_act,tetwin);

    tetwin->key_sig_no=g_signal_connect(G_OBJECT(tetwin->window), "key-release-event",
		     G_CALLBACK(move_cb), tetwin);


}


int
main (int argc, char **argv)
{

  gtk_init (&argc, &argv);



  TetWin *tetwin = tet_window_new ();

random=g_rand_new();



  gtk_widget_show_all (tetwin->window);

/*
    nextshape = tet_shape_new (tetwin->canvas, 0, 0, TET_Z0);
  tet_window_set_preview (tetwin, TET_Z0);
  
    curshape = tet_shape_new (tetwin->canvas, -1, 0, TET_Z1);
    tetwin->shape=curshape;
    

    g_timeout_add(1000,(GSourceFunc)timeout_act,tetwin);


*/


  GtkWidget*but_start=tetwin->start;
  GtkWidget*but_stop=tetwin->stop;
  GtkWidget*but_pause=tetwin->pause;

  g_signal_connect(G_OBJECT(but_start),"clicked",G_CALLBACK(start_cb),tetwin);
  g_signal_connect(G_OBJECT(but_stop),"clicked",G_CALLBACK(stop_cb),tetwin);
  g_signal_connect(G_OBJECT(but_pause),"clicked",G_CALLBACK(pause_cb),tetwin);



  gtk_main ();



  return 0;
}
