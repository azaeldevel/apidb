#include "Application.hpp"
#include "driver.hpp"



namespace octetos
{
namespace apidb
{        
        void on_changed(GtkWidget *widget, gpointer statusbar)
        {
        
                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar *value;

                if (gtk_tree_selection_get_selected(
                GTK_TREE_SELECTION(widget), &model, &iter)) {

                gtk_tree_model_get(model, &iter, 0, &value,  -1);
                gtk_statusbar_push(GTK_STATUSBAR(statusbar),
                        gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
                        value), value);
                g_free(value);
                }
        }
        
        void TreeView::fill()
        {         
                treestore = gtk_tree_store_new(1,G_TYPE_STRING);
                for(std::vector<apidb::ConfigureProject::Table>::iterator it =list.begin(); it != list.end(); it++ )
                {
                        gtk_tree_store_append(treestore, &toplevel, NULL);
                        gtk_tree_store_set(treestore, &toplevel,0, it->getName().c_str(), -1);   
                        for(std::map<const char*, const apidb::ConfigureProject::Function*>::iterator itF = (*it).begin(); itF !=  (*it).end(); itF++)
                        {
                                std::string params = itF->second->listParams();
                                std::string fnProto = itF->second->getName().c_str();
                                fnProto += "(" + params + ")";
                                //std::cout << fnProto << std::endl;
                                gtk_tree_store_append(treestore, &funtion, &toplevel);
                                gtk_tree_store_set(treestore, &funtion,0, fnProto.c_str(), -1);   
                        }
                }
                
                 model = GTK_TREE_MODEL(treestore);
                gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
                g_object_unref(model);  
        }
        
        GtkWidget *TreeView::create() 
        {    
                GtkTreeViewColumn *col;
                GtkCellRenderer *renderer;
                view = gtk_tree_view_new();        

                col = gtk_tree_view_column_new();
                gtk_tree_view_column_set_title(col, "Tablas");
                gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
                
                renderer = gtk_cell_renderer_text_new();
                gtk_tree_view_column_pack_start(col, renderer, TRUE);
                gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

                fill();
                
                return view;                                
        }
        
        TreeView::TreeView(GtkWidget *box,std::vector<ConfigureProject::Table>& l) : list(l)
        {
                GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
                gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 10);                       
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
                gtk_widget_set_size_request(scrolled_window,200,300);
                gtk_container_add (GTK_CONTAINER (box), scrolled_window);       
                                
                GtkTreeSelection *selection; 
                GtkWidget *vbox;
                GtkWidget *statusbar;
  
                vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                gtk_container_add(GTK_CONTAINER(scrolled_window), vbox);
                view = create(); 
                selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
                gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 1); 

                statusbar = gtk_statusbar_new();
                gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);
                GtkWidget * buttonAdd = gtk_button_new_with_label ("Agregar");
                gtk_box_pack_start(GTK_BOX(statusbar), buttonAdd, FALSE, TRUE, 1);

                g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);                
        }
        
        
        Application::Application()
        {
                
        }
        void Application::downloads_addTable (GtkWidget *widget, gpointer   data)
        {
                g_print ("Hello World\n");
        }
                        
        void Application::loadConfig()
        {
                gtk_entry_set_text (GTK_ENTRY(inName),config.getName().c_str());
                gtk_entry_set_text (GTK_ENTRY(inVer),config.getVersion().toString().c_str());
                gtk_combo_box_set_active((GtkComboBox*)inInL,(gint)config.inputLenguaje);
                gtk_combo_box_set_active((GtkComboBox*)inOutL,(gint)config.outputLenguaje);
                gtk_combo_box_set_active((GtkComboBox*)inPkL,(gint)config.packing);
                gtk_combo_box_set_active((GtkComboBox*)inCmpl,(gint)config.compiled);
                
                gtk_entry_set_text (GTK_ENTRY(inLoc),config.conectordb->getHost().c_str());
                gtk_entry_set_text (GTK_ENTRY(inPort),std::to_string(config.conectordb->getPort()).c_str());
                gtk_entry_set_text (GTK_ENTRY(inDB),config.conectordb->getDatabase().c_str());
                gtk_entry_set_text (GTK_ENTRY(inUser),config.conectordb->getUser().c_str());
                gtk_entry_set_text (GTK_ENTRY(inPw),config.conectordb->getPassword().c_str());
                
                /*gtk_combo_box_text_remove_all((GtkComboBoxText*)cmbAddTable);
                gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)cmbAddTable,0);
                octetos::apidb::Driver driver(config);
                if(driver.analyze(false))
                {
                        std::map<const char*,symbols::Tables*,symbols::cmp_str> lst = driver.getAnalyzer().copyListTable();
                        int i = 1;
                        for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = lst.begin(); it != lst.end(); it++)
                        {
                                for(std::list<symbols::Table*>::iterator itJ = (*it).second->begin(); itJ != (*it).second->end(); itJ++)
                                {
                                        gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,i,(*itJ)->fullname.c_str(),(*itJ)->fullname.c_str());        
                                        i++;
                                }
                        }
                }*/
                
                downsTree->fill();
                
                selectsTree->fill();
        }
        
        void Application::createWindow()
        {
                gtk_window_set_title (GTK_WINDOW (window), "APIDB");
                gtk_window_set_default_size (GTK_WINDOW (window), 450, 300);
                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);  
                gtk_container_set_border_width (GTK_CONTAINER (window), 10);   
                gtk_window_set_resizable(GTK_WINDOW (window),FALSE);
                gtk_container_add (GTK_CONTAINER (window), vboxMain);        
        }
        Application* Application::app = NULL;
       char* Application::filename = NULL;
       octetos::apidb::ConfigureProject Application::config;
        void Application::toolbar_chooseDirectory (GtkWidget *widget, gpointer   data)
        {
                GtkWidget *dialog = gtk_file_chooser_dialog_new("Seleccionar Proyecto",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_ACCEPT,NULL);   
                if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
                {
                        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
                        gtk_widget_destroy (dialog);
                        if(!config.readConfig(std::string(filename)))
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Fallo la lectura del archivo de proyecto",
                                                                filename, g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));
                                gtk_widget_destroy (dialog);
                                return;
                        }
                        app->loadConfig();
                        g_free (filename);
                        filename = NULL;
                }
                else
                {
                        gtk_widget_destroy (dialog);
                }
        }
        
        void Application::createToolbar()
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
                GtkToolItem *exit = gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE);
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
                inName = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxName), inName, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxName, FALSE, FALSE,0);
                
                GtkWidget *boxVer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbVer = gtk_label_new ("Version: ");
                gtk_box_pack_start(GTK_BOX(boxVer), lbVer, FALSE, FALSE,0); 
                inVer = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxVer), inVer, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxVer, FALSE, FALSE,0);
                
                GtkWidget *boxInL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbInL = gtk_label_new ("Lenguaje de Entrada:");
                gtk_box_pack_start(GTK_BOX(boxInL), lbInL, FALSE, FALSE,0); 
                inInL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inInL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inInL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inInL,InputLenguajes::MySQL,"mysql","MySQL");
                gtk_box_pack_start(GTK_BOX(boxInL), inInL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxInL, FALSE, FALSE,0);
                
                GtkWidget *boxOutL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbOutL = gtk_label_new ("Lenguaje de Salida:   ");
                gtk_box_pack_start(GTK_BOX(boxOutL), lbOutL, FALSE, FALSE,0); 
                inOutL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inOutL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,OutputLenguajes::CPP,"c++","C++");
                gtk_box_pack_start(GTK_BOX(boxOutL), inOutL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxOutL, FALSE, FALSE,0);        
                
                GtkWidget *boxPkL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbPkL = gtk_label_new ("Empaquetado:   ");
                gtk_box_pack_start(GTK_BOX(boxPkL), lbPkL, FALSE, FALSE,0); 
                inPkL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inPkL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inPkL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inPkL,PackingLenguajes::CMake,"cmake","CMake");
                gtk_box_pack_start(GTK_BOX(boxPkL), inPkL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxPkL, FALSE, FALSE,0);  
                
                GtkWidget *boxCmpl = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbCmpl = gtk_label_new ("Compilado:        ");
                gtk_box_pack_start(GTK_BOX(boxCmpl), lbCmpl, FALSE, FALSE,0); 
                inCmpl = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inCmpl,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,Compiled::STATIC,"static","Estático");
                gtk_box_pack_start(GTK_BOX(boxCmpl), inCmpl, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxCmpl, FALSE, FALSE,0);        
        }

        void Application::createNotebookConexion(GtkWidget *boxConex)
        {
                GtkWidget *boxLoc = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbLoc = gtk_label_new ("Host/IP:");
                gtk_box_pack_start(GTK_BOX(boxLoc), lbLoc, FALSE, FALSE,0); 
                inLoc= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxLoc), inLoc, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxLoc, FALSE, FALSE,0);
                
                GtkWidget *boxPort = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPort = gtk_label_new ("Puerto:");
                gtk_box_pack_start(GTK_BOX(boxPort), lbPort, FALSE, FALSE,0); 
                inPort= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxPort), inPort, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPort, FALSE, FALSE,0);
                
                GtkWidget *boxDB= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbDB = gtk_label_new ("Base de Datos:");
                gtk_box_pack_start(GTK_BOX(boxDB), lbDB, FALSE, FALSE,0); 
                inDB = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxDB), inDB, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxDB, FALSE, FALSE,0);
                
                GtkWidget *boxUser= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbUser = gtk_label_new ("Usuario:");
                gtk_box_pack_start(GTK_BOX(boxUser), lbUser, FALSE, FALSE,0); 
                inUser = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxUser), inUser, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxUser, FALSE, FALSE,0);
                
                GtkWidget *boxPw= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPw = gtk_label_new ("Contraseña:");
                gtk_box_pack_start(GTK_BOX(boxPw), lbPw, FALSE, FALSE,0); 
                inPw = gtk_entry_new();
                gtk_entry_set_visibility(GTK_ENTRY(inPw),FALSE);
                gtk_box_pack_start(GTK_BOX(boxPw), inPw, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPw, FALSE, FALSE,0);
        }
                
        void Application::createNotebook()
        {
                GtkWidget * notebookMain = gtk_notebook_new();
                gchar* strInfo = (gchar*)"Información";
                GtkWidget *boxInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL,6);
                GtkWidget *lbInfo = gtk_label_new (strInfo);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxInfo,lbInfo);
                createNotebookInfo(boxInfo);
                gchar* strConex = (gchar*)"Conexión";
                GtkWidget *boxConex = gtk_box_new (GTK_ORIENTATION_VERTICAL,4);
                GtkWidget * lbConex = gtk_label_new (strConex);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxConex,lbConex);
                createNotebookConexion(boxConex);
                gchar* strDowns = (gchar*)"Descargas";
                boxDowns = gtk_box_new (GTK_ORIENTATION_VERTICAL,1);
                GtkWidget * lbDowns = gtk_label_new (strDowns);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxDowns,lbDowns);
                downsTree =  new TreeView(boxDowns,config.downloads);
                gchar* strSels = (gchar*)"Selecciones";
                boxSelects = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                GtkWidget * lbSels = gtk_label_new (strSels);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxSelects,lbSels);
                selectsTree =  new TreeView(boxSelects,config.selects);
                gtk_box_pack_start(GTK_BOX(vboxMain), notebookMain, FALSE, FALSE,0);
        }
        void  Application::init(int*   argc, char **argv[])
        {
                gtk_init (argc, argv);
        }
                
        void Application::create()
        {
                if(app == NULL)
                {
                        app = this;
                }
                else
                {
                        throw "Solo se permite una instacia del programa.";
                }
                window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
                vboxMain = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                createWindow();        
                        
                toolbar = gtk_toolbar_new();
                createToolbar(); 
                        
                createNotebook();  
                gtk_widget_show_all (window);      
                gtk_main ();
        }
        
}
}
