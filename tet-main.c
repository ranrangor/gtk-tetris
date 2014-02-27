#include<gtk/gtk.h>
#include<glib.h>

#include"tet-canvas.h"
#include"tet-shape.h"

GRand *random;			//=g_rand_new();


gboolean down_cb(TetShape * shape)
{
//    int val=g_rand_int(random)%2;
/*    if(val==0){
    tet_shape_move_left(shape);
    if(COLLISION_FILL==tet_shape_is_collision(shape)){
    tet_shape_move_restore(shape);
    }
    }else{
    tet_shape_move_right(shape);
    if(COLLISION_FILL==tet_shape_is_collision(shape)){
    tet_shape_move_restore(shape);
    }
    }
*/
    tet_shape_move_down(shape);

    CollisionType tt = tet_shape_is_collision(shape);
//    g_print("Type:::%d",tt);
    if (COLLISION_BOTTOM & tt) {
	tet_shape_move_restore(shape);
	return FALSE;
    }
    g_message("to realize...");
    tet_shape_realize(shape);



    g_print("Timeout callback\n.\n.\n");

    return TRUE;
}


gboolean move_cb(GtkWidget * widget, GdkEventKey * event, TetShape * shape)
{

    if (event->type == GDK_KEY_RELEASE) {
	if (event->keyval == GDK_KEY_Left) {
	    shape_print(shape, "BEfore move left");
	    tet_shape_move_left(shape);
	    shape_print(shape, "After move left");
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		shape_print(shape, "Before restore");
		tet_shape_move_restore(shape);
		shape_print(shape, "After restore");
	    } else {

		shape_print(shape, "Before Realize");
		tet_shape_realize(shape);
		shape_print(shape, "After Realize");
	    }

	} else if (event->keyval == GDK_KEY_Right) {

	    tet_shape_move_right(shape);
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		tet_shape_move_restore(shape);
	    } else {

		tet_shape_realize(shape);

	    }
	} else if (event->keyval == GDK_KEY_Down) {

	    shape_print(shape, "BEfore move down");
	    tet_shape_move_down(shape);
	    shape_print(shape, "BEfore move down");
	    if (tet_shape_is_collision(shape) & COLLISION_FILL) {
		shape_print(shape, "BEfore move down");
		tet_shape_move_restore(shape);
		shape_print(shape, "BEfore move down");
	    } else {

		shape_print(shape, "BEfore move down");
		tet_shape_realize(shape);
		shape_print(shape, "BEfore move down");

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



int main(int argc, char **argv)
{

    gtk_init(&argc, &argv);

    random = g_rand_new();
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 300, 600);


    TetCanvas *canvas = tet_canvas_new(CANVAS_HEIGHT,CANVAS_WIDTH);
    tet_canvas_clear_all(canvas);

    TetShape *shape = tet_shape_new(canvas, TET_Z0);

    tet_shape_realize(shape);
//    tet_shape_move_down(shape);
//    tet_shape_move_down(shape);
//    tet_shape_move_down(shape);
//    tet_shape_move_down(shape);
//    tet_shape_move_down(shape);


    g_timeout_add(1000,(GSourceFunc)down_cb,shape);

    g_signal_connect(G_OBJECT(window), "key-release-event",
		     G_CALLBACK(move_cb), shape);


//    shape->x=0;
//    shape->y=0;

    gtk_container_add(GTK_CONTAINER(window), canvas->container);

    gtk_widget_show_all(window);

    gtk_main();



    return 0;
}
