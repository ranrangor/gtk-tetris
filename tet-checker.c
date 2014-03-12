#include"tet-checker.h"




gboolean is_filled(TetChecker * checker, gint i, gint j)
{

    int width=checker->width;
    int height=checker->height;


    if (j >= 0 && i < height&& j < width) {
	if (i < 0)
	    return FALSE;

	g_print("%d.", checker->filling[i*width+j]);
	return checker->filling[i*width+j];

    } else {
	return TRUE;
    }

}











TetChecker *tet_checker_new(int height,int width,int block_siz)
{
    TetChecker *checker = g_slice_new(TetChecker);
    checker->container = gtk_grid_new();	//g_malloc(sizeof(GtkWidget*));
//checker->grid=g_malloc_n(CHECKER_HEIGHT*CHECKER_WIDTH,sizeof(GtkWidget*) );
    checker->height=height;
    checker->width=width;

    checker->grid = (GtkWidget**)g_slice_alloc(height*width*sizeof(GtkWidget*));
    checker->filling=(gboolean*)g_slice_alloc(height*width*sizeof(gboolean));

    

    int i, j;
    for (i = 0; i < height; i++) {

	for (j = 0; j < width; j++) {
	    checker->filling[i*width+j] = FALSE;
	    gchar *w =""; //g_strdup_printf("[%d.%d]", i, j);
	    checker->grid[i*width+j] = gtk_label_new(w);
	   // g_free(w);
	    gtk_grid_attach(GTK_GRID(checker->container),
			    checker->grid[i*width+j], j, i, 1, 1);
	    gtk_widget_set_size_request(checker->grid[i*width+j], block_siz,
					block_siz);
	}

    }

    gtk_widget_show_all(checker->container);

    return checker;
}

void tet_checker_free(TetChecker * checker)
{

//    g_object_unref(checker->container);
//release all source allocated for wigdets that containing in grid.
//
    int width=checker->width;
    int height=checker->height;

    g_slice_free1(height*width*sizeof(GtkWidget*),checker->grid);
    g_slice_free1(height*width*sizeof(gboolean),checker->filling);
    g_object_unref(checker->container);
    g_slice_free(TetChecker, checker);

}


void tet_checker_color_all(TetChecker * checker, gchar * rgba)
{
    int i, j;
    int width=checker->width;
    int height=checker->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    GdkRGBA color;
	    gdk_rgba_parse(&color, rgba);
	    gtk_widget_override_background_color(checker->grid[i*width+j],
						 GTK_STATE_FLAG_NORMAL,
						 &color);

//        checker->filling[i][j]=TRUE;
	}
    }
}


void tet_checker_clear_all(TetChecker * checker)
{
    int i, j;
    int width=checker->width;
    int height=checker->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    GdkRGBA color;
	    gdk_rgba_parse(&color, COLOR_CLEAR);
	    gtk_widget_override_background_color(checker->grid[i*width+j],
						 GTK_STATE_FLAG_NORMAL,
						 &color);
//        checker->filling[i][j]=FALSE;
	}
    }

}


void tet_checker_fill(TetChecker * checker, int i, int j, gboolean fill)
{
    int width=checker->width;
    int height=checker->height;

    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
    checker->filling[i*width+j] = fill;

}

void tet_checker_fill_all(TetChecker * checker,gboolean fill)
{
    int i, j;
    int width=checker->width;
    int height=checker->height;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
        checker->filling[i*width+j]=fill;
	}
    }





}

void tet_checker_color_block(TetChecker * checker, gint i, gint j,
			    gchar * rgba)
{
//    g_return_if_fail(i>=0 ||j>=0);
    int width=checker->width;
    int height=checker->height;
    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
    GtkWidget *blk = checker->grid[i*width+j];
    GdkRGBA color;
    gdk_rgba_parse(&color, rgba);
    g_print("coloring{%d,%d}\n", i, j);
    gtk_widget_override_background_color(blk, GTK_STATE_FLAG_NORMAL,
					 &color);
//    checker->filling[i][j]=TRUE;

}


void tet_checker_clear_block(TetChecker * checker, gint i, gint j)
{
    int width=checker->width;
    int height=checker->height;
    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;

    GtkWidget *blk = checker->grid[i*width+j];
    GdkRGBA color;
    gdk_rgba_parse(&color, COLOR_CLEAR);
    g_print("clearing{%d,%d}\n", i, j);
    gtk_widget_override_background_color(blk, GTK_STATE_FLAG_NORMAL,
					 &color);

//    checker->filling[i][j]=FALSE;

}



/*
gboolean tet_checker_copy_block(TetChecker*checker,int i,int j,int ii,int jj)
{
}

gboolean tet_checker_copy_fill(TetChecker*checker,int i,int j,int ii,int jj)
{
}

*/


int tet_checker_eliminate(TetChecker*checker,int baseline)
{
    int n_eliminated=0;
    g_message("Eliminate..");
    int i, j;
    int width=checker->width;
    int height=checker->height;
    gboolean flag;
    if(baseline>=height)
        baseline=height-1;

    for (i = baseline; i >=0; ) {
    flag=TRUE;
	for (j = 0; j < width; j++) {
	    if(!is_filled(checker,i,j)){
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
            
                if(is_filled(checker,ii,jj)){
                tet_checker_color_block(checker,ii+1,jj,COLOR_FILL);
                tet_checker_fill(checker,ii+1,jj,TRUE);
                }else{
                tet_checker_clear_block(checker,ii+1,jj);
                tet_checker_fill(checker,ii+1,jj,FALSE);
                
                }
//            tet_checker_copy_block(checker,ii,jj,ii+1,jj);
//            tet_checker_copy_fill(checker,ii,jj,ii+1,jj);
            }
        
        }
    
    }else{
    i--;
    }
    }

    return n_eliminated;

}
