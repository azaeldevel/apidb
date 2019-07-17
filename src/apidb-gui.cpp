/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *  author: Azael Reyes
 * */

#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, gpointer   data)
{
        g_print ("Hello World\n");
}

int main (int   argc, char *argv[])
{        
        gtk_init (&argc, &argv);
        GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title (GTK_WINDOW (window), "APIDB");
        gtk_window_set_default_size (GTK_WINDOW (window), 450, 600);
        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);  
        gtk_container_set_border_width (GTK_CONTAINER (window), 10);
        
        GtkWidget *vboxMain = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
        gtk_container_add (GTK_CONTAINER (window), vboxMain);
        
        GtkWidget * toolbar = gtk_toolbar_new();
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
        gtk_box_pack_start(GTK_BOX(vboxMain), toolbar, FALSE, FALSE,0);
        GtkToolItem *build = gtk_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), build, -1);
        GtkToolItem *save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
        GtkToolItem *sep = gtk_separator_tool_item_new();
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 
        GtkToolItem *exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
        g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
        gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_LARGE_TOOLBAR);
        //
        gtk_box_pack_start(GTK_BOX(vboxMain), toolbar, FALSE, FALSE,0);        
        
        //button = gtk_button_new_with_label ("Button 2");
        //g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
        //gtk_box_pack_start(GTK_BOX(vboxMain), button, TRUE, TRUE,3);        
        GtkWidget * notebookMain = gtk_notebook_new();
        gchar* strInfo = "Información";
        GtkWidget *fraInfo = gtk_frame_new("");
        GtkWidget * lbInfo = gtk_label_new (strInfo);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),fraInfo,lbInfo);
        gtk_box_pack_start(GTK_BOX(vboxMain), notebookMain, TRUE, TRUE,3);
        gchar* strConex = "Conexión";
        GtkWidget *fraConex = gtk_frame_new("");
        GtkWidget * lbConex = gtk_label_new (strConex);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),fraConex,lbConex);
        //
        gtk_box_pack_start(GTK_BOX(vboxMain), notebookMain, FALSE, FALSE,0);
        
        
        gtk_widget_show_all (window);
        gtk_main ();
        
        return EXIT_SUCCESS;
}
