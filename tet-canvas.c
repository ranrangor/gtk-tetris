#include"tet-canvas.h"




gboolean is_filled(TetCanvas * canvas, gint i, gint j)
{

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











TetCanvas *tet_canvas_new(int height,int width,int block_siz)
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
	    gchar *w =""; //g_strdup_printf("[%d.%d]", i, j);
	    canvas->grid[i*width+j] = gtk_label_new(w);
	   // g_free(w);
	    gtk_grid_attach(GTK_GRID(canvas->container),
			    canvas->grid[i*width+j], j, i, 1, 1);
	    gtk_widget_set_size_request(canvas->grid[i*width+j], block_siz,
					block_siz);
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

void tet_canvas_fill_all(TetCanvas * canvas,gboolean fill)
{
    int i, j;
    int width=canvas->width;
    int height=canvas->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
        canvas->filling[i*width+j]=fill;
	}
    }





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



/*
gboolean tet_canvas_copy_block(TetCanvas*canvas,int i,int j,int ii,int jj)
{
}

gboolean tet_canvas_copy_fill(TetCanvas*canvas,int i,int j,int ii,int jj)
{
}

*/


int tet_canvas_eliminate(TetCanvas*canvas,int baseline)
{
    int n_eliminated=0;
    g_message("Eliminate..");
    int i, j;
    int width=canvas->width;
    int height=canvas->height;
    gboolean flag;
    if(baseline>=height)
        baseline=height-1;

    for (i = baseline; i >=0; ) {
    flag=TRUE;
	for (j = 0; j < width; j++) {
	    if(!is_filled(canvas,i,j)){
        flag=FALSE;
        break;
        }
	}

    if(flag){//need to eliminat current line,and move all lines beyond to down
        g_message("Line:%d to be eliminating..",i); 
        n_eliminated+=1;
        int ii,jj;
        for (ii=i-1;ii>=0;ii--){
        
            for(jj=0;jj<width;jj++){
            
                if(is_filled(canvas,ii,jj)){
                tet_canvas_color_block(canvas,ii+1,jj,COLOR_FILL);
                tet_canvas_fill(canvas,ii+1,jj,TRUE);
                }else{
                tet_canvas_clear_block(canvas,ii+1,jj);
                tet_canvas_fill(canvas,ii+1,jj,FALSE);
                
                }
//            tet_canvas_copy_block(canvas,ii,jj,ii+1,jj);
//            tet_canvas_copy_fill(canvas,ii,jj,ii+1,jj);
            }
        
        }
    
    }else{
    i--;
    }
    }

    return n_eliminated;

}
