#include<gtk/gtk.h>
#include<glib.h>

#include"tet-canvas.h"
#include"tet-shape.h"
#include"tet-tetris.h"


gboolean timeout_act(TetWin*win)
{

    /**/
    tet_shape_move_down(win->shape);

    CollisionType ctp=tet_shape_is_collision(win->shape);
    if(ctp&COLLISION_FILL){//can not fall anymore
    tet_shape_free(win->shape);
    tet_canvas_eliminate(win->canvas,win->shape->x);
    /*get the next shape active*/
    }







}




int main(int argc, char **argv)
{

    gtk_init(&argc, &argv);



    TetWin*tetwin=tet_window_new();



    tet_window_set_preview(tetwin,TET_Z0);

    gtk_widget_show_all(tetwin->window);








    gtk_main();



    return 0;
}
