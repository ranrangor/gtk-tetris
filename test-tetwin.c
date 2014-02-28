#include<gtk/gtk.h>
#include<glib.h>

#include"tet-canvas.h"
#include"tet-shape.h"
#include"tet-tetris.h"

  TetShape *curshape, *nextshape;

gboolean
timeout_act (TetWin * win)
{

   /**/ tet_shape_move_down (win->shape);
   g_message("move");

  CollisionType ctp = tet_shape_is_collision (win->shape);
  if (ctp & COLLISION_FILL)
    {				//can not fall anymore
        g_message("Detected Filling");
      tet_canvas_eliminate (win->canvas, win->shape->x);
      tet_shape_free (win->shape);
      /*get the next shape active */
      nextshape = tet_shape_new (win->canvas, 0, 0, TET_Z0);
      tet_window_set_preview (win, TET_Z0);


      win->shape = nextshape;

      CollisionType ctpi = tet_shape_is_collision (win->shape);
      if (ctpi & COLLISION_FILL)
	{

        g_message("000Detected Filling");
	  win->overflowed = TRUE;
	}


//      g_message("COLLISION_FILLed");
//      return FALSE;
    }

  if(ctp&COLLISION_BOTTOM){
  
      g_message("COLLISION_BUTTONed");
//      return FALSE;
  
  }


  if (ctp == COLLISION_NONE)
    {

      tet_shape_realize (win->shape);

   g_message("realized");

    }

  if (win->overflowed)
    {
      /*Game Over */

      win->can_oper = FALSE;
      return FALSE;
    }


  return TRUE;

}


void
start_cb (GtkWidget * widget, TetWin * win)
{

  nextshape = tet_shape_new (win->canvas, 0, 0, TET_Z0);
  win->can_oper = TRUE;
  tet_window_set_preview (win, TET_Z0);






}


int
main (int argc, char **argv)
{

  gtk_init (&argc, &argv);



  TetWin *tetwin = tet_window_new ();



  tet_window_set_preview (tetwin, TET_Z0);

  tet_canvas_clear_all (tetwin->canvas);

  gtk_widget_show_all (tetwin->window);


//    nextshape = tet_shape_new (tetwin->canvas, 0, 0, TET_Z0);
    curshape = tet_shape_new (tetwin->canvas, 0, 0, TET_Z0);
    tetwin->shape=curshape;

    g_timeout_add(1000,(GSourceFunc)timeout_act,tetwin);







  gtk_main ();



  return 0;
}
