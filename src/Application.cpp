#include "Application.hpp"




namespace octetos
{
namespace apidb
{
        void Application::createWindow(GtkWidget* vboxMain,GtkWidget *window)
        {
                gtk_window_set_title (GTK_WINDOW (window), "APIDB");
                gtk_window_set_default_size (GTK_WINDOW (window), 450, 300);
                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);  
                gtk_container_set_border_width (GTK_CONTAINER (window), 10);   
                gtk_window_set_resizable(GTK_WINDOW (window),FALSE);
                gtk_container_add (GTK_CONTAINER (window), vboxMain);        
        }
        
       char* Application::filename = NULL;
        void Application::toolbar_chooseDirectory (GtkWidget *widget, gpointer   data)
        {
                GtkWidget *dialog = gtk_file_chooser_dialog_new("Seleccionar Proyecto",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);   
                if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
                {
                        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
                        //open_file (filename);
                        //g_free (filename);
                }

                gtk_widget_destroy (dialog);
        }
        
        void Application::createToolbar(GtkWidget* vboxMain,GtkWidget* toolbar)
        {
                gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
                gtk_box_pack_start(GTK_BOX(vboxMain), toolbar, FALSE, FALSE,0);
                GtkToolItem *open = gtk_tool_button_new_from_stock(GTK_STOCK_DIRECTORY);
                g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(Application::toolbar_chooseDirectory), NULL);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);
                GtkToolItem *build = gtk_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), build, -1);
                GtkToolItem *save = gtk_tool_button_new_from_stock(GTK_STOCK_HARDDISK);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
                GtkToolItem *sep = gtk_separator_tool_item_new();
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 
                GtkToolItem *exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
                g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
                gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_LARGE_TOOLBAR);
                gtk_box_pack_start(GTK_BOX(vboxMain), toolbar, FALSE, FALSE,0);                          
        }


        void Application::createNotebookInfo(GtkWidget *boxInfo)
        {
                GtkWidget *boxName = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbName = gtk_label_new ("Nombre:");
                gtk_box_pack_start(GTK_BOX(boxName), lbName, FALSE, FALSE,0); 
                GtkWidget *inName = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxName), inName, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxName, FALSE, FALSE,0);
                
                GtkWidget *boxVer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbVer = gtk_label_new ("Version: ");
                gtk_box_pack_start(GTK_BOX(boxVer), lbVer, FALSE, FALSE,0); 
                GtkWidget *inVer = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxVer), inVer, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxVer, FALSE, FALSE,0);
                
                GtkWidget *boxInL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbInL = gtk_label_new ("Lenguaje de Entrada:");
                gtk_box_pack_start(GTK_BOX(boxInL), lbInL, FALSE, FALSE,0); 
                GtkWidget *inInL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inInL,1,"mysql","MySQL");
                gtk_box_pack_start(GTK_BOX(boxInL), inInL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxInL, FALSE, FALSE,0);
                
                GtkWidget *boxOutL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbOutL = gtk_label_new ("Lenguaje de Salida:   ");
                gtk_box_pack_start(GTK_BOX(boxOutL), lbOutL, FALSE, FALSE,0); 
                GtkWidget *inOutL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,1,"c++","C++");
                gtk_box_pack_start(GTK_BOX(boxOutL), inOutL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxOutL, FALSE, FALSE,0);        
                
                GtkWidget *boxPkL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbPkL = gtk_label_new ("Empaquetado:   ");
                gtk_box_pack_start(GTK_BOX(boxPkL), lbPkL, FALSE, FALSE,0); 
                GtkWidget *inPkL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inPkL,1,"cmake","CMake");
                gtk_box_pack_start(GTK_BOX(boxPkL), inPkL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxPkL, FALSE, FALSE,0);  
                
                GtkWidget *boxCmpl = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbCmpl = gtk_label_new ("Compilado:        ");
                gtk_box_pack_start(GTK_BOX(boxCmpl), lbCmpl, FALSE, FALSE,0); 
                GtkWidget *inCmpl = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,1,"static","Estatico");
                gtk_box_pack_start(GTK_BOX(boxCmpl), inCmpl, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxCmpl, FALSE, FALSE,0);        
        }

        void Application::createNotebookConexion(GtkWidget *boxConex)
        {
                GtkWidget *boxLoc = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbLoc = gtk_label_new ("Hostname/IP:");
                gtk_box_pack_start(GTK_BOX(boxLoc), lbLoc, FALSE, FALSE,0); 
                GtkWidget *inLoc= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxLoc), inLoc, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxLoc, FALSE, FALSE,0);
                
                GtkWidget *boxPort = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPort = gtk_label_new ("Puerto:");
                gtk_box_pack_start(GTK_BOX(boxPort), lbPort, FALSE, FALSE,0); 
                GtkWidget *inPort= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxPort), inPort, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPort, FALSE, FALSE,0);
                
                GtkWidget *boxDB= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbDB = gtk_label_new ("Base de Datos:");
                gtk_box_pack_start(GTK_BOX(boxDB), lbDB, FALSE, FALSE,0); 
                GtkWidget *inDB = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxDB), inDB, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxDB, FALSE, FALSE,0);
                
                GtkWidget *boxUser= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbUser = gtk_label_new ("Usuario:");
                gtk_box_pack_start(GTK_BOX(boxUser), lbUser, FALSE, FALSE,0); 
                GtkWidget *inUser = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxUser), inUser, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxUser, FALSE, FALSE,0);
                
                GtkWidget *boxPw= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPw = gtk_label_new ("Contraseña:");
                gtk_box_pack_start(GTK_BOX(boxPw), lbPw, FALSE, FALSE,0); 
                GtkWidget *inPw = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxPw), inPw, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPw, FALSE, FALSE,0);
        }
                
        static void activate_addTable (GtkWidget *widget, gpointer   data)
        {
                g_print ("Hello World\n");
        }

        void Application::createNotebookDownloasAddTable()
        {
                GtkWidget* frame = gtk_frame_new("Tabla X");
                gtk_box_pack_end(GTK_BOX(boxDowns), frame, FALSE, FALSE,0);
        }
        
        void Application::createNotebookDownloas()
        {
                btAddTable = gtk_button_new_with_label ("Agregar Descripción");
                g_signal_connect(btAddTable, "clicked", G_CALLBACK (activate_addTable), NULL); 
                gtk_box_pack_end(GTK_BOX(boxDowns), btAddTable, FALSE, FALSE,0);
        }

        void Application::createNotebook(GtkWidget* vboxMain)
        {
                GtkWidget * notebookMain = gtk_notebook_new();
                gchar* strInfo = "Información";
                GtkWidget *boxInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL,6);
                GtkWidget *lbInfo = gtk_label_new (strInfo);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxInfo,lbInfo);
                createNotebookInfo(boxInfo);
                gchar* strConex = "Conexión";
                GtkWidget *boxConex = gtk_box_new (GTK_ORIENTATION_VERTICAL,4);
                GtkWidget * lbConex = gtk_label_new (strConex);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxConex,lbConex);
                createNotebookConexion(boxConex);
                gchar* strDowns = "Descargas";
                boxDowns = gtk_box_new (GTK_ORIENTATION_VERTICAL,1);
                GtkWidget * lbDowns = gtk_label_new (strDowns);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxDowns,lbDowns);
                createNotebookDownloas();
                gchar* strSels = "Selecciones";
                GtkWidget *boxSels = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                GtkWidget * lbSels = gtk_label_new (strSels);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxSels,lbSels);
                gtk_box_pack_start(GTK_BOX(vboxMain), notebookMain, FALSE, FALSE,0);
        }
        void  Application::init(int*   argc, char **argv[])
        {
                gtk_init (argc, argv);
        }
                
        void Application::create()
        {
                window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
                vboxMain = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                createWindow(vboxMain,window);        
                        
                toolbar = gtk_toolbar_new();
                createToolbar(vboxMain,toolbar); 
                        
                createNotebook(vboxMain);  
                gtk_widget_show_all (window);      
                gtk_main ();
        }
        
}
}
