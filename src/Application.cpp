
#include <stdio.h>
#include <string.h>

#include "Application.hpp"
#include "apidb.hpp"


namespace octetos
{
namespace apidb
{      
        
        const char* CaptureParameter::getSelectParam() const
        {
                return strParameter;
        }
        bool CaptureParameter::show()
        {
                gtk_widget_show_all(dialog);
                gint response = gtk_dialog_run(GTK_DIALOG(dialog));
                if(response == GTK_RESPONSE_OK)
                {
                        strParameter = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(cmbAddParameter));
                        gtk_widget_destroy(dialog);
                        return true;
                }
                else if(response == GTK_RESPONSE_CANCEL | response == GTK_RESPONSE_CLOSE)
                {
                        strParameter = NULL;
                        gtk_widget_destroy(dialog);
                        return false;
                }
        }
        CaptureParameter::CaptureParameter(const Driver* d,const char* table) : driver(d)
        {
                dialog = gtk_dialog_new_with_buttons ("Captura de Tabla.", NULL, GTK_DIALOG_MODAL,  GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);                
                //g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
                content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
                label = gtk_label_new ("Seleccione la tabla para Agregar");
                gtk_container_add (GTK_CONTAINER (content_area), label);
                cmbAddParameter = gtk_combo_box_text_new ();
                gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,0,"selecione","Selecione..."); 
                if(driver != NULL)
                {
                        int i = 1;
                        std::cout << "Buscando '" << table  << "' tabla para seleccionar parametros." << std::endl;
                        
                        for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::const_iterator it = driver->getAnalyzer().getListTableConst().begin(); it != driver->getAnalyzer().getListTableConst().end(); it++)
                        {
                                for(std::list<symbols::Table*>::iterator itT = (*it).second->begin(); itT != (*it).second->end(); itT++)
                                {
                                        if((*itT)->name.compare(table) == 0)
                                        {
                                                for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::iterator itP = (*itT)->begin(); itP != (*itT)->end(); itP++)
                                                {
                                                        gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,i,itP->second->name.c_str(),itP->second->name.c_str());        
                                                        i++;                                                
                                                }
                                        }
                                }
                        }
                }
                gtk_combo_box_set_active((GtkComboBox*)cmbAddParameter,0);
                gtk_container_add (GTK_CONTAINER (content_area), cmbAddParameter);
        }
        
        
        
        
        std::string CaptureFuntion::getNameFunction() const
        {
                return strNameFunction;
        }
        void CaptureFuntion::show()
        {
                gtk_widget_show_all(dialog);
                gint response = gtk_dialog_run(GTK_DIALOG(dialog));
                if(response == GTK_RESPONSE_OK) strNameFunction = gtk_entry_get_text( GTK_ENTRY(inAddFunc));
                else strNameFunction == "";
                gtk_widget_destroy(dialog);
        }
        CaptureFuntion::CaptureFuntion(const Driver* d,GtkTreeIter* gtkIt,const char* table) : driver(d)
        {
                
                dialog = gtk_dialog_new_with_buttons ("Captura de Funcion.", NULL, GTK_DIALOG_MODAL,  GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);                
                //g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
                content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
                label = gtk_label_new ("Seleccione la Funcion para Agregar");
                gtk_container_add (GTK_CONTAINER (content_area), label);
                inAddFunc = gtk_entry_new ();
                gtk_container_add (GTK_CONTAINER (content_area), inAddFunc);
                //gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,0,"selecione","Selecione..."); 
                if(driver != NULL)
                {
                        //std::map<const char*,symbols::Tables*,symbols::cmp_str>::const_iterator itTbalbe = std::find(driver->getAnalyzer().getListTableConst().begin(),driver->getAnalyzer().getListTableConst().end(),table);
                        /*int i = 1;
                        for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::const_iterator it = driver->getAnalyzer().getListTableConst().begin(); it != driver->getAnalyzer().getListTableConst().end(); it++)
                        {
                                for(std::list<symbols::Table*>::iterator itJ = (*it).second->begin(); itJ != (*it).second->end(); itJ++)
                                {   
                                        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::iterator itF = (*itJ)->begin(); itF  != (*itJ)->end(); itF++)
                                        {
                                                gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,i,(*itF).first,(*itF).first);     
                                        }
                                        i++;
                                }
                        }*/
                }
                //gtk_combo_box_set_active((GtkComboBox*)cmbAddTable,0);
                //gtk_container_add (GTK_CONTAINER (content_area), cmbAddTable);
        }  
        
        
        
        
        void show_about(GtkWidget *widget, gpointer data) 
        {
                //GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("battery.png", NULL);
                GtkWidget *dialog = gtk_about_dialog_new();
                gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), getPakageName().c_str());
                gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), getPakageVersion().toString().c_str()); 
                //gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Octetos");
                //char  authors[1][30] = {"Azael Reyes"};
                //gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(dialog),(const gchar**)authors);
                gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Hace sencillo crear una API para conectar a tu Base de Datos.");
                gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/azaeldevel/apidb");
                //gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
                //g_object_unref(pixbuf), pixbuf = NULL;
                gtk_dialog_run(GTK_DIALOG (dialog));
                gtk_widget_destroy(dialog);
        }

        const char* CaptureTable::getSelectTable() const
        {
                return table;
        }
        void CaptureTable::show()
        {
                gtk_widget_show_all(dialog);
                gint response = gtk_dialog_run(GTK_DIALOG(dialog));
                if(response == GTK_RESPONSE_OK)
                        table = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(cmbAddTable));
                else
                        table = NULL;
                gtk_widget_destroy(dialog);
        }
        CaptureTable::CaptureTable(const Driver* d,GtkWidget* widget) : driver(d)
        {
                dialog = gtk_dialog_new_with_buttons ("Captura de Tabla.", NULL, GTK_DIALOG_MODAL,  GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);                
                //g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
                content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
                label = gtk_label_new ("Seleccione la tabla para Agregar");
                gtk_container_add (GTK_CONTAINER (content_area), label);
                cmbAddTable = gtk_combo_box_text_new ();
                gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,0,"selecione","Selecione..."); 
                if(driver != NULL)
                {
                        std::map<const char*,symbols::Tables*,symbols::cmp_str> lst = driver->getAnalyzer().copyListTable();
                        int i = 1;
                        for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = lst.begin(); it != lst.end(); it++)
                        {
                                for(std::list<symbols::Table*>::iterator itJ = (*it).second->begin(); itJ != (*it).second->end(); itJ++)
                                {
                                        gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,i,(*itJ)->fullname.c_str(),(*itJ)->fullname.c_str());        
                                        i++;
                                }
                        }
                }
                gtk_combo_box_set_active((GtkComboBox*)cmbAddTable,0);
                gtk_container_add (GTK_CONTAINER (content_area), cmbAddTable);
        }
        
        
                
        void on_changed(GtkWidget *widget, gpointer statusbar)
        {        
                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar *value;

                if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
                {
                        gtk_tree_model_get(model, &iter, 0, &value,  -1);
                        gtk_statusbar_push(GTK_STATUSBAR(statusbar),gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), value), value);
                        g_free(value);
                }
        }
        
        
        
        
        
        
        const char* TreeView::getTableName(GtkTreeModel *model,GtkTreeIter* iter)
        {
                const char* path = gtk_tree_model_get_string_from_iter(model,iter);
                char delim[] = ":";
                char *ptr = strtok((char*)path, delim);
                int count = 0;
                while(ptr != NULL)
                {
                        if(count == 1)
                        {
                               break;
                        }
                        printf("'--%s'\n", ptr);
                        ptr = strtok(NULL, delim);
                        count++;
                }
                
                int nodenumber = atoi(ptr);
                std::map<const char*,ConfigureProject::Table*>::const_iterator it = Application::getConfigure().downloads.begin();
                if(nodenumber <= Application::getConfigure().downloads.size())
                {
                        std::advance(it , nodenumber);
                        std::cout << "Selected table : " << it->second->getName().c_str() << ", node :" << ptr << std::endl;
                        return it->second->getName().c_str();
                }
                else
                {
                        //node de new function
                        std::cout << "New funtion "<<std::endl;
                        return NULL;
                }
        }
        
        char TreeView::checkTypeNode(GtkTreeModel *model,GtkTreeIter* iter)
        {
                const char* path = gtk_tree_model_get_string_from_iter(model,iter);
                char delim[] = ":";
                char *ptr = strtok((char*)path, delim);
                int count = 0;
                while(ptr != NULL)
                {
                        //printf("'%s'\n", ptr);
                        ptr = strtok(NULL, delim);
                        count++;
                }
                 if(count == 3)
                 {
                         return 'F';
                }
                else if(count == 2)
                {
                        return 'T';
                }
                else  if(count == 1)
                {
                        return 'R';
                }
                else
                {
                        return '?';
                }
                        
        }
        void TreeView::row_activated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer  user_data)
        {    
                GtkTreeModel *model;
                GtkTreeIter iter;
                TreeView* wgTree  = (TreeView*)user_data;
                 
                model = gtk_tree_view_get_model(view);

                if (!gtk_tree_model_get_iter(model, &iter, path))
                {
                        std::cout << "No Agregado " << std::endl;
                        actual = NULL;
                        return; /* path describes a non-existing row - should not happen */
                }
                
                const char* name = gtk_tree_model_get_string_from_iter(model,&iter);
                std::cout << "Agregando " << name << std::endl;
                switch(checkTypeNode(model,&iter))
                {
                        case 'T':
                                printf("'%s'\n", "Es una tabla");
                                break;
                        case 'F':
                                printf("'%s'\n", "Es una Funcion");
                                CaptureFuntion cap(Application::getDriver(),&iter,getTableName(model,&iter));
                                cap.show();
                                std::string strFunction = cap.getNameFunction();
                                const char* strTable = getTableName(model,&iter);
                                ConfigureProject::Function* newF = new ConfigureProject::Function(strFunction);
                                std::cout << "Bascando tabla '" << strTable << "'" << std::endl;
                                std::map<const char*,ConfigureProject::Table*>::iterator itT = wgTree->list->find(strTable);
                                if(itT != wgTree->list->end())
                                {
                                        std::cout << "tabla '" << strTable <<  "' encontrada."<< std::endl;
                                        itT->second->insert(std::make_pair(strFunction.c_str(), newF));
                                        bool flag = false;
                                        do
                                        {
                                                CaptureParameter capParams(Application::getDriver(),strTable);
                                                flag = capParams.show();
                                                if(flag)
                                                {
                                                        newF->addParam(capParams.getSelectParam());
                                                }
                                        }
                                        while(flag);
                                        wgTree->fill();
                                }
                                else
                                {
                                        std::cout << "No se encontro la tabla " << strTable << std::endl;
                                }
                                break;
                }              
        }
        void TreeView::fill()
        {         
                treestore = gtk_tree_store_new(1,G_TYPE_STRING);
                //std::cout << "Creating..." << std::endl;
                gtk_tree_store_append(treestore, &toplevel, NULL);
                gtk_tree_store_set(treestore, &toplevel,0, "Tablas", -1); 
                for(std::map<const char*,ConfigureProject::Table*>::iterator it =list->begin(); it != list->end(); it++ )
                {
                        //std::cout << "Table " << it->second->getName().c_str() << std::endl;
                        gtk_tree_store_append(treestore, &table, &toplevel);
                        gtk_tree_store_set(treestore, &table,0, it->second->getName().c_str(), -1);   
                        for(std::map<const char*, const apidb::ConfigureProject::Function*>::iterator itF = it->second->begin(); itF !=  it->second->end(); itF++)
                        {
                                //std::cout << "Funcion " <<  itF->second->getName().c_str() << std::endl;
                                std::string fnProto = itF->second->getName().c_str();
                                if(itF->second->getParameters() != NULL)
                                {
                                        std::string params = itF->second->listParams();
                                        fnProto += "(" + params + ")";
                                }
                                std::cout << fnProto << std::endl;
                                gtk_tree_store_append(treestore, &funtion, &table);
                                gtk_tree_store_set(treestore, &funtion,0, fnProto.c_str(), -1);   
                        }
                        gtk_tree_store_append(treestore, &funtion, &table);
                        gtk_tree_store_set(treestore, &funtion,0, strNewFunct, -1);   
                }
                
                 model = GTK_TREE_MODEL(treestore);
                gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
                g_object_unref(model);  
        }
        
        GtkWidget *TreeView::create() 
        {    
                GtkTreeViewColumn *col;
                GtkCellRenderer *renderer;
                GtkCellRenderer *add;
                view = gtk_tree_view_new();    
                g_signal_connect(G_OBJECT(view), "row-activated", G_CALLBACK(row_activated), this);
                
                col = gtk_tree_view_column_new();
                gtk_tree_view_column_set_title(col, "Tablas");
                gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
                
                renderer = gtk_cell_renderer_text_new();
                gtk_tree_view_column_pack_start(col, renderer, TRUE);
                gtk_tree_view_column_add_attribute(col, renderer, "text", 0);
                                
                fill();
                
                gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view),FALSE);
                return view;                                
        }
        
        TreeView::TreeView(GtkWidget *box,std::map<const char*,ConfigureProject::Table*>* l) : list(l)
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
                g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);  
                
                GtkWidget *pop = gtk_popover_new(statusbar);
                GtkWidget *popvbox = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                gtk_container_add(GTK_CONTAINER(pop), popvbox);
                GtkWidget * mn1 = gtk_button_new_with_label ("menu1");
                gtk_box_pack_start(GTK_BOX(popvbox), mn1, TRUE, TRUE, 1);
                GtkWidget * mn2 = gtk_button_new_with_label ("menu1");
                gtk_box_pack_start(GTK_BOX(popvbox), mn2, TRUE, TRUE, 1);
        }
        

        void Application::createHeader()
        {       
                headerbar = gtk_header_bar_new();
                //gtk_header_bar_set_show_title_buttons (GTK_HEADER_BAR (headerbar), TRUE);
                gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), "APIDB");
                gtk_header_bar_set_has_subtitle (GTK_HEADER_BAR (headerbar), FALSE);
      
                btOpen = gtk_button_new ();
                icoOpen = g_themed_icon_new ("document-open");
                imgOpen = gtk_image_new_from_gicon (icoOpen,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoOpen);
                gtk_container_add (GTK_CONTAINER (btOpen), imgOpen);
                gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), btOpen);
                g_signal_connect(G_OBJECT(btOpen), "clicked", G_CALLBACK(Application::chooseDirectory), NULL);
                
                btNew = gtk_button_new ();
                icoNew= g_themed_icon_new ("document-new");
                imgNew = gtk_image_new_from_gicon (icoNew,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoNew);
                gtk_container_add (GTK_CONTAINER (btNew), imgNew);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btNew);
                
                sep1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), sep1);
                sep2 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), sep2);
                
                btBuild = gtk_button_new ();
                icoBuild = g_themed_icon_new ("system-run");
                imgBuild = gtk_image_new_from_gicon (icoBuild,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoBuild);
                gtk_container_add (GTK_CONTAINER (btBuild), imgBuild);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btBuild);
                
                btSave = gtk_button_new ();
                icoSave= g_themed_icon_new ("document-save");
                imgSave = gtk_image_new_from_gicon (icoSave,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoSave);
                gtk_container_add (GTK_CONTAINER (btSave), imgSave);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btSave);
                              
                
                
                
                btQuit = gtk_button_new ();
                icoQuit= g_themed_icon_new ("application-exit");
                imgQuit = gtk_image_new_from_gicon (icoQuit,GTK_ICON_SIZE_BUTTON);
                g_signal_connect(G_OBJECT(btQuit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
                g_object_unref (icoQuit);
                gtk_container_add (GTK_CONTAINER (btQuit), imgQuit);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), btQuit);       
                
                gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);
                
                gtk_container_add (GTK_CONTAINER (window), gtk_text_view_new ());
                
        }
        
        Driver* Application::getDriver()
        {
                return driver;
        }
        Application* Application::getApplication()
        {
                return app;
        }
        TreeView* Application::getDownloadTreeView()
        {
                return downsTree;
        }
        TreeView* Application::getSelectTreeView()
        {
                return selectsTree;
        }
        const ConfigureProject& Application::getConfigure()
        {
                return config;
        }
        void Application::on_newtable(GtkWidget *widget, gpointer data) 
        {
                if(strcmp(selectedTab, Application::titleDowns) == 0 && flagVisible && driver != NULL)
                {    
                        selectedTab = Application::titleDowns;
                        CaptureTable cap(driver,app->window);
                        cap.show();
                        if(cap.getSelectTable() != NULL)
                        {
                                ConfigureProject::Table* ptb =  new ConfigureProject::Table(cap.getSelectTable());
                                config.downloads.insert(std::make_pair(ptb->getName().c_str(),ptb));
                                app->downsTree->fill();
                        }
                }
                else if(strcmp(selectedTab, Application::titleSelects) == 0 && flagVisible && driver != NULL)
                {
                        CaptureTable cap(driver,app->window);
                        cap.show();
                        //std::cout << "\tTable " << cap.getSelectTable() << std::endl; 
                        if(cap.getSelectTable() != NULL)
                        {
                                ConfigureProject::Table* ptb =  new ConfigureProject::Table(cap.getSelectTable());
                                config.selects.insert(std::make_pair(ptb->getName().c_str(),ptb));
                                app->selectsTree->fill();
                        }
                }            
        }
        void Application::active_tab (GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data)
        {
                if(strcmp(gtk_notebook_get_tab_label_text(notebook,page), Application::titleDowns) == 0 && flagVisible && driver != NULL)
                {
                        //std::cout << "Active " << gtk_notebook_get_tab_label_text(notebook,page) << std::endl;    
                        selectedTab = Application::titleDowns;
                }
                else if(strcmp(gtk_notebook_get_tab_label_text(notebook,page), Application::titleSelects) == 0 && flagVisible && driver != NULL)
                {
                        //std::cout << "Active " << gtk_notebook_get_tab_label_text(notebook,page) << std::endl;           
                        selectedTab = Application::titleSelects;
                }
                else
                {
                        selectedTab = NULL;
                }
        }
        Application::~Application()
        {
                delete downsTree;
                delete selectsTree;
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
        
        
        void Application::chooseDirectory (GtkWidget *widget, gpointer   data)
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
                        if(driver != NULL) 
                        {
                                delete driver;
                                driver = NULL;
                        }
                        driver = new Driver(config);
                        if(driver->analyze(false))
                        {
                                        
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
                g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(Application::chooseDirectory), NULL);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);
                GtkToolItem *build = gtk_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), build, -1);
                GtkToolItem *save = gtk_tool_button_new_from_stock(GTK_STOCK_HARDDISK);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
                GtkToolItem *sep = gtk_separator_tool_item_new();
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 
                GtkToolItem *newTable = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
                g_signal_connect(G_OBJECT(newTable), "clicked", G_CALLBACK(on_newtable), NULL);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), newTable, -1);
                GtkToolItem *sep2 = gtk_separator_tool_item_new();
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep2, -1); 
                GtkToolItem *about = gtk_tool_button_new_from_stock(GTK_STOCK_ABOUT);
                gtk_toolbar_insert(GTK_TOOLBAR(toolbar), about, -1);
                g_signal_connect(G_OBJECT(about), "clicked", G_CALLBACK(show_about), NULL);
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
                g_signal_connect(G_OBJECT(notebookMain), "switch-page", G_CALLBACK(active_tab), window);
                GtkWidget *boxInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL,6);
                GtkWidget *lbInfo = gtk_label_new (titleInfo);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxInfo,lbInfo);
                createNotebookInfo(boxInfo);
                GtkWidget *boxConex = gtk_box_new (GTK_ORIENTATION_VERTICAL,4);
                GtkWidget * lbConex = gtk_label_new (titleConex);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxConex,lbConex);
                createNotebookConexion(boxConex);
                boxDowns = gtk_box_new (GTK_ORIENTATION_VERTICAL,1);
                GtkWidget * lbDowns = gtk_label_new (titleDowns);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxDowns,lbDowns);
                downsTree =  new TreeView(boxDowns,&config.downloads);
                boxSelects = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                GtkWidget * lbSels = gtk_label_new (titleSelects);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxSelects,lbSels);
                selectsTree =  new TreeView(boxSelects,&config.selects);
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
                                
                createHeader();
                
                createNotebook();  
                gtk_widget_show_all (window);   
                flagVisible = true;
                gtk_main ();
        }

         const char*  Application::titleInfo = "Información";
         const char*  Application::titleConex = "Conexión";
         const char*  Application::titleDowns = "Descargas";
         const char*  Application::titleSelects = "Selecciones";
         bool Application::flagVisible = false;
         Driver* Application::driver = NULL;
         const char*  Application::selectedTab = NULL;
        GtkTreeIter* TreeView::actual = NULL;
        const char* TreeView::strNewFunct = "Agregar Funcion";
        Application* Application::app = NULL;
        char* Application::filename = NULL;
        octetos::apidb::ConfigureProject Application::config;
}
}
