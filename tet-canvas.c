#include"tet-canvas.h"





gboolean is_filled(TetCanvas * canvas, gint i, gint j)
{
    /*
       if(i<0||j<0){
       return TRUE;
       }else if(i>=CANVAS_HEIGHT){
       return TRUE;
       }else if(j>=CANVAS_WIDTH){
       return TRUE;
       }else{
       return can->filling[i][j];
       }
     */

    int width=canvas->width;
    int height=canvas->height;


    if (j >= 0 && i < height&& j < width) {
	if (i < 0)
	    return FALSE;

	g_print("%d.", canvas->filling[i*width+j]);
	return canvas->filling[i*width+j];

    } else {
	return TRUE;
    }

}








TetCanvas *tet_canvas_new(int height,int width)
{
    TetCanvas *canvas = g_slice_new(TetCanvas);
    canvas->container = gtk_grid_new();	//g_malloc(sizeof(GtkWidget*));
//canvas->grid=g_malloc_n(CANVAS_HEIGHT*CANVAS_WIDTH,sizeof(GtkWidget*) );
    canvas->height=height;
    canvas->width=width;

    canvas->grid = (GtkWidget**)g_slice_alloc(height*width*sizeof(GtkWidget*));
    canvas->filling=(gboolean*)g_slice_alloc(height*width*sizeof(gboolean));

    

    int i, j;
    for (i = 0; i < height; i++) {

	for (j = 0; j < width; j++) {
	    canvas->filling[i*width+j] = FALSE;
	    gchar *w = g_strdup_printf("[%d.%d]", i, j);
	    canvas->grid[i*width+j] = gtk_label_new(w);
	    g_free(w);
	    gtk_grid_attach(GTK_GRID(canvas->container),
			    canvas->grid[i*width+j], j, i, 1, 1);
	    gtk_widget_set_size_request(canvas->grid[i*width+j], BLOCK_SIZE,
					BLOCK_SIZE);
	}

    }

    gtk_widget_show_all(canvas->container);

    return canvas;
}

void tet_canvas_free(TetCanvas * canvas)
{

//    g_object_unref(canvas->container);
//release all source allocated for wigdets that containing in grid.
//
    int width=canvas->width;
    int height=canvas->height;

    g_slice_free1(height*width*sizeof(GtkWidget*),canvas->grid);
    g_slice_free1(height*width*sizeof(gboolean),canvas->filling);
    g_object_unref(canvas->container);
    g_slice_free(TetCanvas, canvas);

}


void tet_canvas_color_all(TetCanvas * canvas, gchar * rgba)
{
    int i, j;
    int width=canvas->width;
    int height=canvas->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    GdkRGBA color;
	    gdk_rgba_parse(&color, rgba);
	    gtk_widget_override_background_color(canvas->grid[i*width+j],
						 GTK_STATE_FLAG_NORMAL,
						 &color);

//        canvas->filling[i][j]=TRUE;
	}
    }
}


void tet_canvas_clear_all(TetCanvas * canvas)
{
    int i, j;
    int width=canvas->width;
    int height=canvas->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    GdkRGBA color;
	    gdk_rgba_parse(&color, COLOR_CLEAR);
	    gtk_widget_override_background_color(canvas->grid[i*width+j],
						 GTK_STATE_FLAG_NORMAL,
						 &color);
//        canvas->filling[i][j]=FALSE;
	}
    }

}


void tet_canvas_fill(TetCanvas * canvas, int i, int j, gboolean fill)
{
    int width=canvas->width;
    int height=canvas->height;

    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
    canvas->filling[i*width+j] = fill;

}


void tet_canvas_color_block(TetCanvas * canvas, gint i, gint j,
			    gchar * rgba)
{
//    g_return_if_fail(i>=0 ||j>=0);
    int width=canvas->width;
    int height=canvas->height;
    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
    GtkWidget *blk = canvas->grid[i*width+j];
    GdkRGBA color;
    gdk_rgba_parse(&color, rgba);
    g_print("coloring{%d,%d}\n", i, j);
    gtk_widget_override_background_color(blk, GTK_STATE_FLAG_NORMAL,
					 &color);
//    canvas->filling[i][j]=TRUE;

}


void tet_canvas_clear_block(TetCanvas * canvas, gint i, gint j)
{
    int width=canvas->width;
    int height=canvas->height;
    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;

    GtkWidget *blk = canvas->grid[i*width+j];
    GdkRGBA color;
    gdk_rgba_parse(&color, COLOR_CLEAR);
    g_print("clearing{%d,%d}\n", i, j);
    gtk_widget_override_background_color(blk, GTK_STATE_FLAG_NORMAL,
					 &color);

//    canvas->filling[i][j]=FALSE;

}
