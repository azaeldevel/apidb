/**
 * 
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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
 * */



#include "Application.hpp"

int main (int   argc, char *argv[])
{                                        
        octetos::apidb::Application app;
        app.init (&argc, &argv);                   
        app.create();        
        return EXIT_SUCCESS;
}

/*int main (int argc, char *argv[])
{
    GtkWidget *wndMain;
    GtkBuilder *builder = NULL;

    gtk_init (&argc , &argv);

    builder = gtk_builder_new();

    if( gtk_builder_add_from_file (builder,"/home/azael/develop/octetos/apidb.pub.src/src/apidb.glade" , NULL) == 0)
    {
        printf("gtk_builder_add_from_file FAILED\n");
        return(0);
    }

    wndMain  = GTK_WIDGET (gtk_builder_get_object (builder,"wndMain"));

    gtk_builder_connect_signals(builder,NULL);

    gtk_widget_show_all (wndMain);
    gtk_main ();
    return 0;
}*/
