#include"tet-checker.h"



typedef struct _block{
GtkDrawingArea parent;
GQuark color;
int siz;

}Block;


typedef struct _blockClass{
GtkDrawingAreaClass parent_class;


}BlockClass;


enum {
PROP_0,
PROP_SIZ,
PROP_COLOR

};





#define TYPE_BLOCK (block_get_type())
#define BLOCK(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),TYPE_BLOCK,Block))
#define BLOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),TYPE_BLOCK,BlockClass))

G_DEFINE_TYPE(Block,block,GTK_TYPE_DRAWING_AREA)



static void block_init(Block*blk)
{


}


static void block_get_property(GObject*obj,guint prop_id,GValue *value, GParamSpec* pspec)
{
Block*bobj=BLOCK(obj);

switch(prop_id)
{
    case PROP_SIZ:
        g_value_set_int(value,bobj->siz);
        break;
    case PROP_COLOR:
        g_value_set_string(value,g_quark_to_string(bobj->color));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(obj,prop_id,pspec);
        break;

}

}


static void block_set_property(GObject*obj,guint prop_id,const GValue *value, GParamSpec* pspec)
{
Block*bobj=BLOCK(obj);

switch(prop_id)
{
    case PROP_SIZ:
        bobj->siz=g_value_get_int(value);
        gtk_widget_set_size_request(GTK_WIDGET(obj),bobj->siz,bobj->siz);
        break;
    case PROP_COLOR:
        bobj->color=g_quark_from_string(g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(obj,prop_id,pspec);
        break;

}
gtk_widget_queue_draw(GTK_WIDGET(obj));

}


static gboolean block_draw(GtkWidget*w,cairo_t* cr)
{

    Block*blk=BLOCK(w);

    GdkRGBA color;
    gdk_rgba_parse(&color,g_quark_to_string(blk->color));
    gdk_cairo_set_source_rgba(cr,&color);

    cairo_rectangle(cr,0,0,blk->siz,blk->siz);
    cairo_set_line_width(cr,1);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr,0.1,0.1,0.1);
    cairo_stroke(cr);
    return FALSE;



}




static void block_class_init(BlockClass*klass)
{

    GtkWidgetClass* wclass=GTK_WIDGET_CLASS(klass);
    GObjectClass*gclass=G_OBJECT_CLASS(klass);

    gclass->set_property=block_set_property;
    gclass->get_property=block_get_property;

    wclass->draw=block_draw;



    g_object_class_install_property(gclass,PROP_SIZ,
                g_param_spec_int("size","Size","size of block",1,100,10,G_PARAM_READWRITE));


    g_object_class_install_property(gclass,PROP_COLOR,
                g_param_spec_string("color","Color-Desc","Quark of color descripter for parse",COLOR_CLEAR,G_PARAM_READWRITE));




}

void block_color(Block*blk,char*color_desc)
{
    g_object_set(G_OBJECT(blk),"color",color_desc,NULL);
}



void block_clear(Block*blk)
{
    g_object_set(G_OBJECT(blk),"color",COLOR_CLEAR,NULL);
}


void block_set_size(Block*blk,int size)
{
    g_object_set(G_OBJECT(blk),"size",size,NULL);
}


void block_get_color(Block*blk,char**color)
{
g_object_get(G_OBJECT(blk),"color",&*color,NULL);
}



GtkWidget* newBlock(gchar*color,int siz)
{
return (GtkWidget*)g_object_new(TYPE_BLOCK,"color",color,"size",siz,NULL);
}




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
    checker->container = gtk_grid_new();

    checker->height=height;
    checker->width=width;

    checker->grid = (GtkWidget**)g_slice_alloc(height*width*sizeof(GtkWidget*));
    checker->filling=(gboolean*)g_slice_alloc(height*width*sizeof(gboolean));
    
    checker->bsiz=block_siz;
    

    int i, j;
    for (i = 0; i < height; i++) {

	for (j = 0; j < width; j++) {
	    checker->filling[i*width+j] = FALSE;
	    checker->grid[i*width+j] =newBlock(COLOR_CLEAR,block_siz);
	    gtk_grid_attach(GTK_GRID(checker->container),
			    checker->grid[i*width+j], j, i, 1, 1);
	}

    }

    gtk_widget_show_all(checker->container);

    return checker;
}

void tet_checker_free(TetChecker * checker)
{

//release all source allocated for wigdets that containing in grid.
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
        block_color(BLOCK(checker->grid[i*width+j]),rgba);
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

        block_clear(BLOCK(checker->grid[i*width+j]));
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
    int width=checker->width;
    int height=checker->height;

    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
        block_color(BLOCK(checker->grid[i*width+j]),rgba);
    //    checker->filling[i][j]=TRUE;

}


void tet_checker_clear_block(TetChecker * checker, gint i, gint j)
{
    int width=checker->width;
    int height=checker->height;

    if (i < 0 || j < 0 || i >= height|| j >= width)
	return;
    
        block_clear(BLOCK(checker->grid[i*width+j]));
    //    checker->filling[i][j]=FALSE;

}



int tet_checker_eliminate(TetChecker*checker,int baseline)
{
    int n_eliminated=0;
    g_message("Eliminating..");
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
//        g_message("Line:%d to be eliminating..",i); 
        n_eliminated+=1;
        int ii,jj;
        for (ii=i-1;ii>=0;ii--){
            for(jj=0;jj<width;jj++){
            
                if(is_filled(checker,ii,jj)){
                char *blkcolor;
                block_get_color(BLOCK(checker->grid[ii*width+jj]),&blkcolor);
                tet_checker_color_block(checker,ii+1,jj,blkcolor);
                tet_checker_fill(checker,ii+1,jj,TRUE);
                }else{
                tet_checker_clear_block(checker,ii+1,jj);
                tet_checker_fill(checker,ii+1,jj,FALSE);
                
                }
            }
        
        }
    
    }else{
    i--;
    }
    }

    return n_eliminated;

}
