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

#include <stdio.h>
#include <string.h>
#include <toolkit/clientdb/clientdb-mysql.hpp>
#include <list>


#include "Application-gtk3.hpp"
#include "apidb.hpp"
#include "Errors.hpp"
#include "versionInfo.h"


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
                else
                {
                        strParameter = NULL;
                        gtk_widget_destroy(dialog);
                        return false;
                }
                
                return false;
        }
	CaptureParameter::CaptureParameter(const Driver* d,const char* table) : driver(d)
	{
		dialog = gtk_dialog_new_with_buttons ("Captura de Parametro.", NULL, GTK_DIALOG_MODAL,  "gtk-ok", GTK_RESPONSE_OK, NULL);                
		//g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
		content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
		label = gtk_label_new ("Seleccione la tabla para Agregar");
		gtk_container_add (GTK_CONTAINER (content_area), label);
		cmbAddParameter = gtk_combo_box_text_new ();
		gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,0,"selecione","Selecione..."); 
		if(driver != NULL)
		{ 
			int i = 0;
			std::list<std::string> list;
			driver->getFiledsName(list,table);
			for(std::list<std::string>::iterator it = list.begin(); it != list.end(); it++,i++)
			{
				gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,i,(*it).c_str(),(*it).c_str()); 
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
                else strNameFunction = "";
                gtk_widget_destroy(dialog);
        }
        CaptureFuntion::CaptureFuntion(const Driver* d,GtkTreeIter* gtkIt,const char* table) : driver(d)
        {
                dialog = gtk_dialog_new_with_buttons ("Captura de Funcion.", NULL, GTK_DIALOG_MODAL,  "gtk-ok", GTK_RESPONSE_OK, NULL);                
                //g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
                content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
                label = gtk_label_new ("Seleccione la Funcion para Agregar");
                gtk_container_add (GTK_CONTAINER (content_area), label);
                inAddFunc = gtk_entry_new ();
                gtk_container_add (GTK_CONTAINER (content_area), inAddFunc);
        }  
        
        
        

        const char* CaptureTable::getSelectTable() const
        {
                return table;
        }
        bool CaptureTable::show()
        {
                gtk_widget_show_all(dialog);
                gint response = gtk_dialog_run(GTK_DIALOG(dialog));
                if(response == GTK_RESPONSE_OK)
                {
                        table = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(cmbAddTable));
                        gtk_widget_destroy(dialog);
                        return true;
                }
                else
                {
                        table = NULL;
                        gtk_widget_destroy(dialog);
                        return false;
                } 
        }
	CaptureTable::CaptureTable(const Driver* d) : driver(d)
	{
		dialog = gtk_dialog_new_with_buttons ("Captura de Tabla.", NULL, GTK_DIALOG_MODAL,  "gtk-ok", GTK_RESPONSE_OK, NULL);                
		//g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
		content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
		label = gtk_label_new ("Seleccione la tabla para Agregar");
		gtk_container_add (GTK_CONTAINER (content_area), label);
		cmbAddTable = gtk_combo_box_text_new ();
		gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,0,"selecione","Selecione..."); 
		if(driver != NULL)
		{
			int i = 0;
			std::list<std::string> list;
			d->getTablesName(list);
			for(std::list<std::string>::iterator it = list.begin(); it != list.end(); it++,i++)
			{
				gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddTable,i,(*it).c_str(),(*it).c_str());
			}
		}
		gtk_combo_box_set_active((GtkComboBox*)cmbAddTable,0);
		gtk_container_add (GTK_CONTAINER (content_area), cmbAddTable);
	}
        
        
        
        
        
        
        
        Application* TreeView::getApplication()const
        {
                return app;
        }
        const char* TreeView::getTableName(GtkTreeModel *model,GtkTreeIter* iter,std::map<const char*,ConfigureProject::Table*,symbols::cmp_str>* list)
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
                        //printf("'--%s'\n", ptr);
                        ptr = strtok(NULL, delim);
                        count++;
                }
                
                int nodenumber = atoi(ptr);
                std::map<const char*,ConfigureProject::Table*>::const_iterator it = list->begin();
                if(nodenumber <= list->size())
                {
                        std::advance(it , nodenumber);
                        //std::cout << "Selected table : " << it->second->getName().c_str() << ", node :" << ptr << std::endl;
                        return it->second->getName().c_str();
                }
                else
                {
                        //node de new function
                        //std::cout << "New funtion "<<std::endl;
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
                        //std::cout << "No Agregado " << std::endl;
                        //actual = NULL;
                        return; /* path describes a non-existing row - should not happen */
                }
                
                const char* name = gtk_tree_model_get_string_from_iter(model,&iter);
                //std::cout << "Agregando " << name << std::endl;
                switch(checkTypeNode(model,&iter))
                {
                        case 'R':
                                {
                                        CaptureTable capT(wgTree->getApplication()->getDriver());
                                        if(capT.show())
                                        {
                                                ConfigureProject::Table* ptb =  new ConfigureProject::Table(capT.getSelectTable());
                                                wgTree->list->insert(std::make_pair(ptb->getName().c_str(),ptb));
                                                wgTree->fill();
                                                wgTree->getApplication()->setSaved(false);
                                        }
                                }
                                break;
                        case 'T':
                                {
                                        CaptureFuntion cap(wgTree->getApplication()->getDriver(),&iter,getTableName(model,&iter,wgTree->list));
                                        cap.show();
                                        std::string strFunction = cap.getNameFunction();
                                        const char* strTable = getTableName(model,&iter,wgTree->list);
                                        ConfigureProject::Function* newF = new ConfigureProject::Function(strFunction);
                                        //std::cout << "Bascando tabla '" << strTable << "'" << std::endl;
                                        std::map<const char*,ConfigureProject::Table*>::iterator itT = wgTree->list->find(strTable);
                                        if(itT != wgTree->list->end())
                                        {
                                                //std::cout << "tabla '" << strTable <<  "' encontrada."<< std::endl;
                                                itT->second->insert(std::make_pair(strFunction.c_str(), newF));
                                                bool flag = false;
                                                do
                                                {
                                                        CaptureParameter capParams(wgTree->getApplication()->getDriver(),strTable);
                                                        flag = capParams.show();
                                                        if(flag)
                                                        {
                                                                newF->addParam(capParams.getSelectParam());
                                                                wgTree->getApplication()->setSaved(false);
                                                        }
                                                        else
                                                        {
                                                                break;
                                                        }
                                                }
                                                while(flag);
                                                wgTree->fill();
                                        }
                                        else
                                        {
                                                //std::cout << "No se encontro la tabla " << strTable << std::endl;
                                                //std::cout << "Tabla size:" <<  wgTree->list->size() << std::endl;
                                                std::string msgstr = "Fallo interno, no se encontro la tabla busca ";
                                                msgstr = msgstr + strTable + "'";
                                                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::APPLICATION_GTK3_ROWACTIVE_NOTFOUND_TABLE,__FILE__,__LINE__));
                                        }
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
                                //std::cout << fnProto << std::endl;
                                gtk_tree_store_append(treestore, &funtion, &table);
                                gtk_tree_store_set(treestore, &funtion,0, fnProto.c_str(), -1);   
                        }
                        //gtk_tree_store_append(treestore, &funtion, &table);
                        //gtk_tree_store_set(treestore, &funtion,0, strNewFunct, -1);   
                }
                
                 model = GTK_TREE_MODEL(treestore);
                gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
                g_object_unref(model);  
        }
        
        GtkWidget *TreeView::create() 
        {    
                GtkTreeViewColumn *col;
                GtkCellRenderer *renderer;
                //GtkCellRenderer *add;
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
        
        TreeView::TreeView(GtkWidget *box,std::map<const char*,ConfigureProject::Table*,symbols::cmp_str>* l,Application* a) : list(l), app(a)
        {
                GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
                gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 10);                       
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
                gtk_widget_set_size_request(scrolled_window,200,300);
                gtk_container_add (GTK_CONTAINER (box), scrolled_window);       
                                
                GtkTreeSelection *selection; 
                GtkWidget *vbox;
                //GtkWidget *statusbar;
  
                vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                gtk_container_add(GTK_CONTAINER(scrolled_window), vbox);
                view = create(); 
                selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
                gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 1); 
        }
        

	bool Application::downConf()
	{
                if(isSaved and !isOpen) return true;
                
                if(inNameEdited)
                {
                        std::string name = gtk_entry_get_text(GTK_ENTRY(inName));
                        if(name.empty())
                        {
                                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "El nombre del proyecto no puede estar vacio",
                                                                        "Error", g_strerror (errno));
                                        gtk_dialog_run (GTK_DIALOG (msg));       
                                        gtk_widget_destroy (msg);                          
                                return false;
                        }
                        config->name = name;
                }
                if(inInLEdited)
                {
                        int innIn = gtk_combo_box_get_active(GTK_COMBO_BOX(inInL));
                        if(innIn == 0)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Selecione un lenguajde de entrada(MySQL por ejemplo) en la sección de información",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));                                  
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        config->inputLenguaje = getInputLenguaje(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inInL)));
                }
                if(inOutLEdited)
                {
                        int intout = gtk_combo_box_get_active(GTK_COMBO_BOX(inOutL));
                        if(intout == 0)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Selecione un lenguajde de salida(C++ por ejemplo) en la sección de información",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        config->outputLenguaje =  getOutputLenguajes(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inOutL)));
                }
                if(inVerEdited)
                {
                        std::string verstr = gtk_entry_get_text(GTK_ENTRY(inVer));
                        if(!config->versionResult.fromString(verstr))
                        {                                       
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Fallo la lectura del la version, el formato aceptable es 'x.y.z'",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                        }                                
                }
                if(inPkLEdited)
                {
                        int intpkl = gtk_combo_box_get_active(GTK_COMBO_BOX(inPkL));
                        if(intpkl == 0)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Selecione un opción de empaquetado en la seccion de información.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                                
                        }
                        config->packing = getPackingLenguajes(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inPkL)));
                }
                if(inCmplEdited)
                {
                        int intcmpl = gtk_combo_box_get_active(GTK_COMBO_BOX(inCmpl));
                        if(intcmpl == 0)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Selecione un opción de empaquetado en la seccion de información.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                                
                        }
                        config->compiled = getCompiled(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inCmpl)));
                }
		if(inNameSpaceDetectEdited)
		{
			int intNameSpaceDetect = gtk_combo_box_get_active(GTK_COMBO_BOX(inNameSpaceDetect));
			if(intNameSpaceDetect == 0)
			{
				GtkWidget *msg = gtk_message_dialog_new (NULL,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"Selecione 'Emular' si dese aceptar espacion en C++ en la sección de información");
				gtk_dialog_run (GTK_DIALOG (msg));  
				gtk_widget_destroy (msg);
				return false;
			}
			config->namespace_detect = gtk_combo_box_get_active_id(GTK_COMBO_BOX(inNameSpaceDetect));
		}
                
                
                if(config->conectordb == NULL)
                {
                        if(config->inputLenguaje == InputLenguajes::MySQL)
                        {
                                        config->conectordb = new toolkit::clientdb::mysql::Datconnect();
                        }
                        else
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Indique primero el lenguaje de la Base de Datos",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));    
                                gtk_widget_destroy (msg);
                                return false;
                        }
                }
                if(inLocEdited)
                {
                        std::string loc = gtk_entry_get_text(GTK_ENTRY(inLoc));
                        if(loc.empty())
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Indique la direcion del host o sitio web en la seccion de Conexión.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        if(toolkit::clientdb::Connector::is_ipv4_address(loc))
                        {
                                config->conectordb->setHost(loc);                                
                        }
                        else if(toolkit::clientdb::Connector::is_ipv6_address(loc))
                        {
                                config->conectordb->setHost(loc);    
                        }
                        else if(toolkit::clientdb::Connector::is_valid_domain_name(loc))
                        {
                                config->conectordb->setHost(loc);    
                        }
                        else
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_WARNING,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "La dirección indica no parece ser validad, quiza tendrá que corregirlo despues.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));   
                                gtk_widget_destroy (msg);
                        }
                }
                if(inPortEdited)
                {
                        std::string strport = gtk_entry_get_text(GTK_ENTRY(inPort));
                        if(strport.empty())
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Indique el número de puerto.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        unsigned int intport = std::stoi(strport.c_str());
                        if(intport < 1)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "El número de puesto deve er un entero positivo.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        config->conectordb->setPort(intport);
                }
                if(inDBEdited)
                {
                        std::string strdb = gtk_entry_get_text(GTK_ENTRY(inDB));
                        if(strdb.empty())
                        {                                
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Indique una base de datos.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));   
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        config->conectordb->setDatabase(strdb);
                }
                if(inUserEdited)
                {
                        std::string struser = gtk_entry_get_text(GTK_ENTRY(inUser));
                        if(struser.empty())
                        {                                
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Indique un nombre de usuario para la Base de Datos.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));   
                                gtk_widget_destroy (msg);
                                return false;
                        }
                        config->conectordb->setUser(struser);
                }
                if(inPwEdited)
                {
                        std::string strpw = gtk_entry_get_text(GTK_ENTRY(inPw));   
                        if(strpw.empty())
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_WARNING,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Dejo vacio el campo de contraseña.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));    
                                gtk_widget_destroy (msg);
                        }
                        config->conectordb->setPassword(strpw);
                }
                
                
		return true;
	}
	void Application::build(GtkWidget *widget, gpointer data) 
	{
                Application* app = (Application*)data;
                
                if(app->config == NULL)
                {
                        if(app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Cree o abre un proyecto",
                                                                "Al construir el proyecto", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                                return;
                        }
                        else
                        {
                                std::string msgstr = "";
                                if(toolkit::Error::check())
                                {
                                                msgstr = toolkit::Error::get().what();
                                }
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        msgstr.c_str(),
                                                                        msgstr.c_str(), g_strerror (errno));
                                        gtk_dialog_run (GTK_DIALOG (msg));    
                                        gtk_widget_destroy (msg);
                                        return;
                        }
                        std::string msgstr = "Algo anda mal, no hay configuracion de proyecto.";
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        msgstr.c_str(),
                                                                        msgstr.c_str(), g_strerror (errno));
                                        gtk_dialog_run (GTK_DIALOG (msg));    
                                        gtk_widget_destroy (msg);
                                        return;
                }
                
                if(app->driver != NULL) 
                {
                        delete (app->driver);
                        app->driver = NULL;
                }
                app->driver = new Driver(*(app->config));
                Tracer tr(0);
		if(!app->driver->driving(&tr))
		{
			std::string msgstr = "";
			if(toolkit::Error::check())
			{
				msgstr = toolkit::Error::get().what();
			}
			else
			{
				msgstr ="Fallo durante la construccion.";
			}
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);
			return;
		}
		else
		{
                        std::string strmsg = "La generación del proyecto es correct, véa su resultado en '";
						strmsg += app->getConfigure()->builDirectory + "'.\nPara construir rápida use los camando comunes:\n\t\tcmake . && make";
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_INFO,
                                                                GTK_BUTTONS_CLOSE,
                                                                strmsg.c_str());
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
		}
	}
        void Application::document_saveas(GtkWidget *widget, gpointer data) 
        {
                Application* app = (Application*)data;
                
                if(app->config == NULL)
                {
                        if(!app->isOpen)
                        {//no esta abierto el proyecto.
                                
                                return;
                        }
                        
                        //causo desconocida para este error
                        return;
                }
                               
                if(!app->downConf())
                {
                        //ocurrio un error en downConf, ya se presento el mensaje al usuario simpleme termina esta llama.
                        return;
                }
                GtkWidget *dialog;
                GtkFileChooser *chooser;
                GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
                gint res;

                dialog = gtk_file_chooser_dialog_new ("Save File",
                                                NULL,
                                                action,
                                                "_Cancel",
                                                GTK_RESPONSE_CANCEL,
                                                "_Save",
                                                GTK_RESPONSE_ACCEPT,
                                                NULL);
                chooser = GTK_FILE_CHOOSER (dialog);
               gtk_file_chooser_set_current_name (chooser,(app->config->name + ".apidb").c_str());
                res = gtk_dialog_run (GTK_DIALOG (dialog));
                if (res == GTK_RESPONSE_ACCEPT)
                {
                        char *filename;

                        filename = gtk_file_chooser_get_filename (chooser);
                        app->config->saveConfig(filename);
                        app->isOpen = true;
                        app->isNew = false;
                        app->setSaved(true);
                        g_free (filename);
                }
                gtk_widget_destroy (dialog);                
        }
        void Application::document_save(GtkWidget *widget, gpointer data) 
        {
                Application* app = (Application*)data;
                
                if(app->config == NULL)
                {
                        if(!app->isOpen || !app->isSaved)
                        {//no esta abierto el proyecto.                                
                                std::string msgstr;
                                if(toolkit::Error::check())
                                {
                                        msgstr = toolkit::Error::get().what();
                                }
                                else
                                {
                                        msgstr = "No hay documento abierto o pendiente de guardar";
                                }
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                msgstr.c_str(),
                                                                "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                                return;
                        }
                        
                                                       
                        std::string msgstr;
                        if(toolkit::Error::check())
                        {
                                        msgstr = toolkit::Error::get().what();
                        }
                        else
                        {
                                        msgstr = "Algo anda mal, no hay configuracionde proyecto cargada";
                        }
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                msgstr.c_str(),
                                                                "Error", g_strerror (errno));
                        gtk_dialog_run (GTK_DIALOG (msg)); 
                        gtk_widget_destroy (msg);
                        return;
                }
                
                if(!app->originFilename.empty() and !app->isNew) //si fue cargado simplemete usa el mismo archivo
                {
                        //escribir en la estura de configuracion del proyecto.
                        if(!app->downConf())
                        {
                                std::string msgstr;
                                if(toolkit::Error::check())
                                {
                                        msgstr = toolkit::Error::get().what();
                                }
                                else
                                {
                                        msgstr = "Ocurrio un erro desconocido la operacion de guardar el archivo.";
                                }
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                msgstr.c_str(),
                                                                "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                                return;
                        }
                        //Guarda los datos en disco
                        if(!app->config->saveConfig(app->originFilename))
                        {
                                std::string msgstr;
                                if(toolkit::Error::check())
                                {
                                        msgstr = toolkit::Error::get().what();
                                }
                                else
                                {
                                        msgstr = "Ocurrio un erro desconocido la operacion de guardar el archivo.";
                                }
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                msgstr.c_str(),
                                                                "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                                return;
                        }
                        app->isOpen = true;
                        app->setSaved(true);
                        return;
                }
                else if(app->isNew)//ya se sabe que no fue cargado desde el disco
                {
                        document_saveas(widget,data);                          
                }
                else
                {
                        //????????
                }
        }
        void Application::document_new(GtkWidget *widget, gpointer data) 
        {
                Application* app = (Application*)data;
                if(app->isOpen)
                {
                        if(!app->isSaved)//solicitar confirmacion de Gaurdar/Cerrar proyecto.
                        {
                                
                                //ahora simplemete no realiza la operacion
                                return;
                        }
                }
                
                app->config = new ConfigureProject();
                app->createNotebook();
                app->isOpen = true;
                app->isNew = true;
                app->setSaved(false);
                app->originFilename = "";
                gtk_widget_show_all (app->window); 
        }
        void Application::show_about(GtkWidget *widget, gpointer data) 
        {
                GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/usr/shared/octetos/apidb/apidb-128x128.png", NULL);
                GtkWidget *dialog = gtk_about_dialog_new();
                gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), PAKAGENAME_DECORED);
                gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), getPakageVersion().toString().c_str()); 
                gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog), "APIDB  Copyright (C) 2018  Azael Reyes \n This program comes with ABSOLUTELY NO WARRANTY.");
                const gchar *authors[] = {"Azael Reyes<azael.devel@gmail.com>", NULL};
                const gchar *documenters[] = {"Azael Reyes<azael.devel@gmail.com>", NULL};
                gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(dialog),(const gchar**)authors);
                gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (dialog), documenters);
                gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Hace sencillo crear una API para conectar tu Base de Datos.");
                gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/azaeldevel/apidb");
                gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
                g_object_unref(pixbuf), pixbuf = NULL;
                gtk_dialog_run(GTK_DIALOG (dialog));
                gtk_widget_destroy(dialog);
        }
        void Application::document_close(GtkWidget *widget, gpointer data) 
        {
                Application* app = (Application*)data;
                if(app->isSaved)
                {                        
                        if(app->notebookMain != NULL)
                        {
                                gtk_widget_destroy(app->notebookMain);
                                app->notebookMain = NULL;
                        }
                        if(app->downsTree != NULL)
                        {
                                delete (app->downsTree);
                                app->downsTree = NULL;
                        }
                        if(app->selectsTree != NULL)
                        {
                                delete (app->selectsTree);
                                app->selectsTree = NULL;
                        }
                }                
                app->isOpen = false;
                app->isSaved = false;
                app->originFilename = "";
        }
        void Application::setSaved(bool saved)
        {
                isSaved = saved;
                if(isSaved)
                {
                        gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), nameApp.c_str());
                        gtk_window_set_title (GTK_WINDOW (window), nameApp.c_str());
                }
                else
                {
                        gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), (nameApp + "*").c_str());
                        gtk_window_set_title (GTK_WINDOW (window),(nameApp + "*").c_str());
                }
        }
        void Application::createHeader()
        {                       
                headerbar = gtk_header_bar_new();
                //gtk_header_bar_set_show_title_buttons (GTK_HEADER_BAR (headerbar), TRUE);
                gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), nameApp.c_str());
                gtk_header_bar_set_has_subtitle (GTK_HEADER_BAR (headerbar), FALSE);
                
                btOpen = gtk_button_new ();
                icoOpen = g_themed_icon_new ("document-open");
                imgOpen = gtk_image_new_from_gicon (icoOpen,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoOpen);
                gtk_container_add (GTK_CONTAINER (btOpen), imgOpen);
                gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), btOpen);
                g_signal_connect(G_OBJECT(btOpen), "clicked", G_CALLBACK(Application::document_open), this);
                
                btNew = gtk_button_new ();
                icoNew= g_themed_icon_new ("document-new");
                imgNew = gtk_image_new_from_gicon (icoNew,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoNew);
                gtk_container_add (GTK_CONTAINER (btNew), imgNew);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btNew);
                g_signal_connect(G_OBJECT(btNew), "clicked", G_CALLBACK(Application::document_new), this);
                
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
                g_signal_connect(G_OBJECT(btBuild), "clicked", G_CALLBACK(Application::build), this);
                
                btSave = gtk_button_new ();
                icoSave= g_themed_icon_new ("document-save");
                imgSave = gtk_image_new_from_gicon (icoSave,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoSave);
                gtk_container_add (GTK_CONTAINER (btSave), imgSave);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btSave);
                g_signal_connect(G_OBJECT(btSave), "clicked", G_CALLBACK(Application::document_save), this);
                              
                btSaveAs = gtk_button_new ();
                icoSaveAs= g_themed_icon_new ("document-save-as");
                imgSaveAs = gtk_image_new_from_gicon (icoSaveAs,GTK_ICON_SIZE_BUTTON);
                g_object_unref (icoSaveAs);
                gtk_container_add (GTK_CONTAINER (btSaveAs), imgSaveAs);
                gtk_header_bar_pack_start(GTK_HEADER_BAR (headerbar), btSaveAs);
                g_signal_connect(G_OBJECT(btSaveAs), "clicked", G_CALLBACK(Application::document_saveas), this);
                
                                
                btQuit = gtk_button_new ();
                icoQuit= g_themed_icon_new ("application-exit");
                imgQuit = gtk_image_new_from_gicon (icoQuit,GTK_ICON_SIZE_BUTTON);
                g_signal_connect(G_OBJECT(btQuit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
                g_object_unref (icoQuit);
                gtk_container_add (GTK_CONTAINER (btQuit), imgQuit);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), btQuit);       
                
                sep3 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), sep3);
                
                btAbout = gtk_button_new ();
                icoAbout= g_themed_icon_new ("help-about");
                imgAbout = gtk_image_new_from_gicon (icoAbout,GTK_ICON_SIZE_BUTTON);
                g_signal_connect(G_OBJECT(btAbout), "clicked", G_CALLBACK(show_about), NULL);
                g_object_unref (icoAbout);
                gtk_container_add (GTK_CONTAINER (btAbout), imgAbout);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), btAbout);  
                
                sep4 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), sep4);
                
                btCloseDoc = gtk_button_new ();
                icoCloseDoc= g_themed_icon_new ("document-close");
                imgCloseDoc = gtk_image_new_from_gicon (icoCloseDoc ,GTK_ICON_SIZE_BUTTON);
                g_signal_connect(G_OBJECT(btCloseDoc), "clicked", G_CALLBACK(document_close), this);
                g_object_unref (icoCloseDoc);
                gtk_container_add (GTK_CONTAINER (btCloseDoc), imgCloseDoc);
                gtk_header_bar_pack_end(GTK_HEADER_BAR (headerbar), btCloseDoc); 
                                
                gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);
        }
        
        Driver* Application::getDriver()
        {
                return driver;
        }
        TreeView* Application::getDownloadTreeView()
        {
                return downsTree;
        }
        TreeView* Application::getSelectTreeView()
        {
                return selectsTree;
        }
        ConfigureProject* Application::getConfigure()const
        {
                return config;
        }
        Application::~Application()
        {
                if(downsTree != NULL)
                {
                        delete downsTree;
                        downsTree = NULL;
                }
                if(selectsTree != NULL)
                {
                        delete selectsTree;
                        selectsTree = NULL;
                }
                if(config != NULL)
                {
                        delete config;
                        config = NULL;
                }
        }
        Application::Application()
        {
                isSaved = false;
                isOpen = false;
                downsTree = NULL;
                selectsTree = NULL;
                config = NULL;
                driver = NULL;
                conexEdited = false;
                inLocEdited = false;
                inPortEdited = false;
                inDBEdited = false;
                inUserEdited = false;
                inPwEdited = false;
                inNameEdited = false;
                inVerEdited = false;
                inInLEdited = false;
                inOutLEdited=false;
                inPkLEdited = false;
                inCmplEdited = false;
                isNew = false;
        }   
        
        void Application::loadConfig()
        {
                gtk_entry_set_text (GTK_ENTRY(inName),config->name.c_str());
                gtk_entry_set_text (GTK_ENTRY(inVer),config->versionResult.toString().c_str());
                gtk_combo_box_set_active(GTK_COMBO_BOX(inInL),(gint)config->inputLenguaje);
                gtk_combo_box_set_active(GTK_COMBO_BOX(inOutL),(gint)config->outputLenguaje);
                gtk_combo_box_set_active(GTK_COMBO_BOX(inPkL),(gint)config->packing);
                gtk_combo_box_set_active(GTK_COMBO_BOX(inCmpl),(gint)config->compiled);
                gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (inFileChooserBuildDirectory),config->builDirectory.c_str());
                gtk_combo_box_set_active(GTK_COMBO_BOX(inNameSpaceDetect),inNameSpaceDetect_comboxid(config->namespace_detect));
                
                gtk_entry_set_text (GTK_ENTRY(inLoc),config->conectordb->getHost().c_str());
                gtk_entry_set_text (GTK_ENTRY(inPort),std::to_string(config->conectordb->getPort()).c_str());
                gtk_entry_set_text (GTK_ENTRY(inDB),config->conectordb->getDatabase().c_str());
                gtk_entry_set_text (GTK_ENTRY(inUser),config->conectordb->getUser().c_str());
                gtk_entry_set_text (GTK_ENTRY(inPw),config->conectordb->getPassword().c_str());
                                
                downsTree->fill();
                
                selectsTree->fill();
                
        }
        void Application::application_destroy (GtkWidget *object, gpointer   user_data)
        {
                
        }
        void Application::createWindow()
        {
                gtk_window_set_title (GTK_WINDOW (window), nameApp.c_str());
                gtk_window_set_default_size (GTK_WINDOW (window), 450, 300);
                g_signal_connect (window, "destroy", G_CALLBACK (application_destroy), this);  
                gtk_container_set_border_width (GTK_CONTAINER (window), 10);   
                gtk_window_set_resizable(GTK_WINDOW (window),FALSE);
                gtk_container_add (GTK_CONTAINER (window), vboxMain);       
        }  
	void Application::documen_open(Application* app,const std::string& filefly)
	{
		std::string filename;
		
		if(filefly.empty())
		{
			GtkWidget *dialog = gtk_file_chooser_dialog_new("Seleccionar Proyecto",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_ACCEPT,NULL);   
			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				char* tmpfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
				filename = tmpfilename;
				gtk_widget_destroy (dialog);
				g_free (tmpfilename);
			}
			gtk_widget_destroy (dialog);
		}
		else		
		{
			filename = filefly;
		}
		std::cout << "Step 1 "<< std::endl;
		if(app->config ==  NULL)
		{
			app->config = new ConfigureProject();
		}
		else
		{
			delete (app->config);
			app->config = new ConfigureProject();
		}
                        
		std::cout << "Step 2" << std::endl;
		try
		{
			if(!app->config->readConfig(std::string(filename)))
			{                 
				std::string msgstr = "";
				GtkWidget *msg;
				if(toolkit::Error::check())
				{
					toolkit::Error e = toolkit::Error::get();
					msgstr = e.what();
					msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,msgstr.c_str());
				}
				else
				{
					msgstr ="Fallo la lectura del archivo de proyecto";
					msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
				}
				gtk_dialog_run (GTK_DIALOG (msg));
				return;
			} 
		}
		catch(std::exception e)
		{
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE, e.what());
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);
			return;
		}
		std::cout << "Step 3" << std::endl;
		if(app->driver != NULL) 
		{
			delete (app->driver);
			app->driver = NULL;
		}
		std::cout << "Step 3.5" << std::endl;
		app->driver = new Driver(*(app->config));
		std::string msgstr = "";
		if(toolkit::Error::check())
		{
			toolkit::Error e = toolkit::Error::get();
			msgstr = e.what();			
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,msgstr.c_str());
			gtk_dialog_run (GTK_DIALOG (msg));
			gtk_widget_destroy (msg);
			return;
		}
		std::cout << "Step 4" << std::endl;
		if(!app->driver->analyze(NULL))
		{
			std::string msgstr = "";
			if(toolkit::Error::check())
			{
				msgstr = toolkit::Error::get().what();
			}
			else
			{
				msgstr ="Fallo la lectura del archivo de proyecto";
			}
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);                                
		}
		std::cout << "Step 5" << std::endl;
		app->originFilename = filename;
		app->createNotebook();
		app->loadConfig();
		app->setSaved(true);
		app->isOpen = true;
		app->isNew = false;
		gtk_widget_show_all(app->window);
	}
	void Application::document_open (GtkWidget *widget, gpointer   data)
	{
		Application* app = (Application*)data;
		documen_open(app);
	}
        gboolean Application::inVer_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return)
                {                        
                        if(app->config != NULL)
                        {
                                app->inVerEdited = true;
                                if(app->isSaved)app->setSaved(false);
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                                return true;
                        }
                }
                return FALSE;
        }
        gboolean Application::inName_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                 
                if(app->config != NULL)
                {
                                app->inNameEdited = true;
                                if(app->isSaved)app->setSaved(false);
                }
                else if(!app->isOpen)
                {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                }
                
                return FALSE;
        }
        void Application::inInL_changed (GtkComboBox *widget, gpointer     user_data)
        {
                Application* app = (Application*) user_data;
                if(app->config != NULL)
                {
                        //std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
                        //std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
                        app->inInLEdited = true;
                        app->setSaved(false);
                }
                else if(!app->isOpen)
                {
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Deve crear un proyecto antes de captura informacion.",
                                                                "Error", g_strerror (errno));
                        gtk_dialog_run (GTK_DIALOG (msg)); 
                        gtk_widget_destroy (msg);
                        return;
                }
        }
        void Application::inOutL_changed (GtkComboBox *widget, gpointer     user_data)
        {
                Application* app = (Application*) user_data;
                if(app->config != NULL)
                {
                        //std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
                        //std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
                        app->inOutLEdited = true;
                        app->setSaved(false);
                }
                else if(!app->isOpen)
                {
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Deve crear un proyecto antes de captura informacion.",
                                                                "Error", g_strerror (errno));
                        gtk_dialog_run (GTK_DIALOG (msg)); 
                        gtk_widget_destroy (msg);
                        return;
                }
        }
        void Application::inPkL_changed (GtkComboBox *widget, gpointer     user_data)
        {
                Application* app = (Application*) user_data;
                if(app->config != NULL)
                {
                        //std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
                        //std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
                        app->inPkLEdited = true;
                        app->setSaved(false);
                }
                else if(!app->isOpen)
                {
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Deve crear un proyecto antes de captura informacion.",
                                                                "Error", g_strerror (errno));
                        gtk_dialog_run (GTK_DIALOG (msg)); 
                        gtk_widget_destroy (msg);
                        return;
                }
        }
        void Application::inCmpl_changed (GtkComboBox *widget, gpointer     user_data)
        {
                Application* app = (Application*) user_data;
                if(app->config != NULL)
                {                        
                        //std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
                        //std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
                        app->inCmplEdited = true;
                        app->setSaved(false);
                }
                else if(!app->isOpen)
                {
                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_ERROR,
                                                                GTK_BUTTONS_CLOSE,
                                                                "Deve crear un proyecto antes de captura informacion.",
                                                                "Error", g_strerror (errno));
                        gtk_dialog_run (GTK_DIALOG (msg)); 
                        gtk_widget_destroy (msg);
                        return;
                }
        }
        void Application::buildDirectory_fileset (GtkFileChooserButton *widget,gpointer user_data)
        {
                Application* app = (Application*) user_data;
                
                char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (app->inFileChooserBuildDirectory));
                app->config->builDirectory = filename;
                //std::cout << "File set = " << filename << std::endl;
                app->setSaved(false);
                g_free (filename); 
        }
	void Application::inNameSpaceDetect_changed (GtkComboBox *widget, gpointer     user_data)
	{
		Application* app = (Application*) user_data;
		if(app->config != NULL)
		{
			std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
			std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
			app->inNameSpaceDetectEdited = true;
			app->setSaved(false);
		}
		else if(!app->isOpen)
		{
			GtkWidget *msg = gtk_message_dialog_new (NULL,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"Deve crear un proyecto antes de captura informacion.",
				"Error", g_strerror (errno));
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);
			return;
		}
	}
        void Application::createNotebookInfo(GtkWidget *boxInfo)
        {
                GtkWidget *boxName = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbName = gtk_label_new ("Nombre:");
                gtk_box_pack_start(GTK_BOX(boxName), lbName, FALSE, FALSE,0); 
                inName = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxName), inName, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxName, FALSE, FALSE,0);
                gtk_widget_set_events(inName,GDK_KEY_PRESS_MASK);
                g_signal_connect(G_OBJECT(inName), "key-press-event", G_CALLBACK(inName_keypress), this);
                
                GtkWidget *boxVer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbVer = gtk_label_new ("Version: ");
                gtk_box_pack_start(GTK_BOX(boxVer), lbVer, FALSE, FALSE,0); 
                inVer = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxVer), inVer, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxVer, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inVer), "key-press-event", G_CALLBACK(inVer_keypress), this);
                
                GtkWidget *boxInL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbInL = gtk_label_new ("Lenguaje de Entrada:");
                gtk_box_pack_start(GTK_BOX(boxInL), lbInL, FALSE, FALSE,0); 
                inInL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inInL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inInL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inInL,InputLenguajes::MySQL,getInputLenguaje(InputLenguajes::MySQL).c_str(),getInputLenguaje(InputLenguajes::MySQL).c_str());
                gtk_box_pack_start(GTK_BOX(boxInL), inInL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxInL, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inInL), "changed", G_CALLBACK(inInL_changed), this);
                
                GtkWidget *boxOutL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbOutL = gtk_label_new ("Lenguaje de Salida:   ");
                gtk_box_pack_start(GTK_BOX(boxOutL), lbOutL, FALSE, FALSE,0); 
                inOutL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inOutL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,OutputLenguajes::C,getOutputLenguajes(OutputLenguajes::C).c_str(),getOutputLenguajes(OutputLenguajes::C).c_str());
                gtk_combo_box_text_insert((GtkComboBoxText*)inOutL,OutputLenguajes::CPP,getOutputLenguajes(OutputLenguajes::CPP).c_str(),getOutputLenguajes(OutputLenguajes::CPP).c_str());
                gtk_box_pack_start(GTK_BOX(boxOutL), inOutL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxOutL, FALSE, FALSE,0);        
                g_signal_connect(G_OBJECT(inOutL), "changed", G_CALLBACK(inOutL_changed), this);
                
                GtkWidget *boxPkL = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbPkL = gtk_label_new ("Empaquetado:   ");
                gtk_box_pack_start(GTK_BOX(boxPkL), lbPkL, FALSE, FALSE,0); 
                inPkL = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inPkL,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inPkL,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inPkL,PackingLenguajes::CMake,getPackingLenguajes(PackingLenguajes::CMake).c_str(),getPackingLenguajes(PackingLenguajes::CMake).c_str());
                gtk_box_pack_start(GTK_BOX(boxPkL), inPkL, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxPkL, FALSE, FALSE,0);  
                g_signal_connect(G_OBJECT(inPkL), "changed", G_CALLBACK(inPkL_changed), this);
                
                GtkWidget *boxCmpl = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbCmpl = gtk_label_new ("Compilado:        ");
                gtk_box_pack_start(GTK_BOX(boxCmpl), lbCmpl, FALSE, FALSE,0); 
                inCmpl = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,0,"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inCmpl,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,Compiled::STATIC,getCompiled(Compiled::STATIC).c_str(),getCompiled(Compiled::STATIC).c_str());
                gtk_combo_box_text_insert((GtkComboBoxText*)inCmpl,Compiled::SHARED,getCompiled(Compiled::SHARED).c_str(),getCompiled(Compiled::SHARED).c_str());
                gtk_box_pack_start(GTK_BOX(boxCmpl), inCmpl, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxCmpl, FALSE, FALSE,0);        
                g_signal_connect(G_OBJECT(inCmpl), "changed", G_CALLBACK(inCmpl_changed), this);
                
                GtkWidget *boxinFileChooserBuildDirectory = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbinFileChooserBuildDirectory = gtk_label_new ("Directory de Contrucción:");
                gtk_box_pack_start(GTK_BOX(boxinFileChooserBuildDirectory), lbinFileChooserBuildDirectory, FALSE, FALSE,0); 
                inFileChooserBuildDirectory = gtk_file_chooser_button_new ("Seleccionar Directorio.",GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
                //gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (inFileChooserBuildDirectory),config->builDirectory.c_str());
                gtk_box_pack_start(GTK_BOX(boxinFileChooserBuildDirectory), inFileChooserBuildDirectory, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxinFileChooserBuildDirectory, FALSE, FALSE,0);
                gtk_widget_set_events(inFileChooserBuildDirectory,GDK_KEY_PRESS_MASK);
                g_signal_connect(G_OBJECT(inFileChooserBuildDirectory), "file-set", G_CALLBACK(buildDirectory_fileset), this);
				
                GtkWidget *boxNameSpaceDetect = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbNameSpaceDetect = gtk_label_new ("Deteción de nombre de espacio:   ");
                gtk_box_pack_start(GTK_BOX(boxNameSpaceDetect), lbNameSpaceDetect, FALSE, FALSE,0); 
                inNameSpaceDetect = gtk_combo_box_text_new();
                gtk_combo_box_text_insert((GtkComboBoxText*)inNameSpaceDetect,inNameSpaceDetect_comboxid("selecione"),"selecione","Selecione..."); 
                gtk_combo_box_set_active((GtkComboBox*)inNameSpaceDetect,0);
                gtk_combo_box_text_insert((GtkComboBoxText*)inNameSpaceDetect,inNameSpaceDetect_comboxid("reject"),"reject","Rechazar");
                gtk_combo_box_text_insert((GtkComboBoxText*)inNameSpaceDetect,inNameSpaceDetect_comboxid("emulate"),"emulate","Emular");
                gtk_box_pack_start(GTK_BOX(boxNameSpaceDetect), inNameSpaceDetect, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxNameSpaceDetect, FALSE, FALSE,0);        
                g_signal_connect(G_OBJECT(inNameSpaceDetect), "changed", G_CALLBACK(inNameSpaceDetect_changed), this);
        }
        int Application::inNameSpaceDetect_comboxid(const std::string& str)
		{
			if(str.compare("selecione") == 0)
			{
				return 0;
			}
			else if(str.compare("reject") == 0)
			{
				return 1;
			}
			else if(str.compare("emulate") == 0)
			{
				return 2;
			}
		}
        gboolean Application::inLoc_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return)
                {                        
                        if(app->config != NULL)
                        {                               
                                //std::cout << "Loc: " << app->config->conectordb->getHost() << std::endl;
                                if(app->isSaved)app->setSaved(false);
                                app->inLocEdited = true;
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                }
                }
                return FALSE;
        }
        gboolean Application::inPort_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return )
                {                        
                        if(app->config != NULL)
                        {
                                if(app->isSaved)app->setSaved(false);
                                app->inPortEdited = true;
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                }
                }
                return FALSE;
        }
        gboolean Application::inDB_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return )
                {                        
                        if(app->config != NULL)
                        {                               
                                if(app->isSaved)app->setSaved(false);
                                app->inDBEdited = true;
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                }
                }
                return FALSE;
        }
        gboolean Application::inUser_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return)
                {                        
                        if(app->config != NULL)
                        {                                
                                if(app->isSaved)app->setSaved(false);
                                app->inUserEdited = true;
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                gtk_widget_destroy (msg);
                }
                }
                return FALSE;
        }
        gboolean Application::inPw_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                
                //if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_ISO_Enter || event->keyval ==GDK_KEY_Return)
                {                        
                        if(app->config != NULL)
                        {                                
                                if(app->isSaved)app->setSaved(false);
                                app->inPwEdited = true;
                        }
                        else if(!app->isOpen)
                        {
                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        "Deve crear un proyecto antes de captura informacion.",
                                                                        "Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                        gtk_widget_destroy (msg);
                }
                }
                return FALSE;
        }
        void Application::createNotebookConexion(GtkWidget *boxConex)
        {
                GtkWidget *boxLoc = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbLoc = gtk_label_new ("Host/IP:");
                gtk_box_pack_start(GTK_BOX(boxLoc), lbLoc, FALSE, FALSE,0); 
                inLoc= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxLoc), inLoc, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxLoc, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inLoc), "key-press-event", G_CALLBACK(inLoc_keypress), this);
                
                GtkWidget *boxPort = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPort = gtk_label_new ("Puerto:");
                gtk_box_pack_start(GTK_BOX(boxPort), lbPort, FALSE, FALSE,0); 
                inPort= gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxPort), inPort, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPort, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inPort), "key-press-event", G_CALLBACK(inPort_keypress), this);
                
                GtkWidget *boxDB= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbDB = gtk_label_new ("Base de Datos:");
                gtk_box_pack_start(GTK_BOX(boxDB), lbDB, FALSE, FALSE,0); 
                inDB = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxDB), inDB, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxDB, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inDB), "key-press-event", G_CALLBACK(inDB_keypress), this);
                
                GtkWidget *boxUser= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbUser = gtk_label_new ("Usuario:");
                gtk_box_pack_start(GTK_BOX(boxUser), lbUser, FALSE, FALSE,0); 
                inUser = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxUser), inUser, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxUser, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inUser), "key-press-event", G_CALLBACK(inUser_keypress), this);
                
                GtkWidget *boxPw= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget *lbPw = gtk_label_new ("Contraseña:");
                gtk_box_pack_start(GTK_BOX(boxPw), lbPw, FALSE, FALSE,0); 
                inPw = gtk_entry_new();
                gtk_entry_set_visibility(GTK_ENTRY(inPw),FALSE);
                gtk_box_pack_start(GTK_BOX(boxPw), inPw, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxConex), boxPw, FALSE, FALSE,0);
                g_signal_connect(G_OBJECT(inPw), "key-press-event", G_CALLBACK(inPw_keypress), this);
        }

        void Application::conex_switchPage (GtkNotebook *notebook, GtkWidget   *page, guint page_num, gpointer     user_data)
        {
                static int newPage=-1;
                static int prePage = -1;
                Application* app = (Application*) user_data;
                
                if(newPage > -1) prePage = newPage;                
                newPage = page_num;
                
                if(prePage == 1 and newPage != 1) //si estaba en la pagina de conexion
                {
                        //std::cout << "Switch from " << prePage << " to " << page_num << std::endl;
                        if(app->inLocEdited || app->inPortEdited || app->inDBEdited || app->inUserEdited || app->inPwEdited)
                        {
                                app->conexEdited = true;
                                if(app->isSaved) app->setSaved(false);
                                if(app->isNew)  app->downConf();
                                try
                                {
                                        //std::cout << app->config->conectordb->getHost() << std::endl;
                                        //std::cout << app->config->conectordb->getDatabase() << std::endl;
                                        //std::cout << app->config->conectordb->getPort() << std::endl;
                                        //std::cout << app->config->conectordb->getPassword() << std::endl;
                                        if(!app->config->testConexion())
                                        {                                        
                                                GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                                GTK_MESSAGE_ERROR,
                                                                                GTK_BUTTONS_CLOSE,
                                                                                "Fallo la conexion a la Base de datos revise sus parametros de conexion.",
                                                                                "Error", g_strerror (errno));
                                                gtk_dialog_run (GTK_DIALOG (msg)); 
                                                gtk_widget_destroy (msg);
                                                return;
                                        }
                                }
                                catch(octetos::toolkit::clientdb::SQLException e)
                                {
                                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        e.what(),
                                                                        "Error detectado", g_strerror (errno));
                                        gtk_dialog_run (GTK_DIALOG (msg)); 
                                        gtk_widget_destroy (msg);
                                        return;
                                }
                                 if(app->driver == NULL)
                                 {
                                        app->driver = new Driver(*(app->config));
                                }
                                else
                                {
                                        delete (app->driver);
                                        app->driver = new Driver(*(app->config));
                                }
                                if(!app->driver->analyze(NULL))
                                {
                                        std::string msgstr = "";
                                        if(toolkit::Error::check())
                                        {
                                                msgstr = toolkit::Error::get().what();
                                        }
                                        else
                                        {
                                                msgstr ="Fallo la lectura del archivo de proyecto";
                                        }
                                        GtkWidget *msg = gtk_message_dialog_new (NULL,
                                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                        GTK_MESSAGE_ERROR,
                                                                        GTK_BUTTONS_CLOSE,
                                                                        msgstr.c_str(),
                                                                        filename, g_strerror (errno));
                                        gtk_dialog_run (GTK_DIALOG (msg));     
                                        gtk_widget_destroy (msg);
                                }
                        }
                }
        }
        bool Application::createNotebook()
        {                
                if(config == NULL)
                {
                        std::string msg = "Deve cargar del proyecto. '";
                        toolkit::Error::write(toolkit::Error(msg,ErrorCodes::APPLICATION_GTK3_CONFIGPROJECT_NULL));
                        return false;
                }
                
                notebookMain = gtk_notebook_new();
                //g_signal_connect(G_OBJECT(notebookMain), "switch-page", G_CALLBACK(active_tab), window);
                GtkWidget *boxInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL,8);
                GtkWidget *lbInfo = gtk_label_new (titleInfo);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxInfo,lbInfo);
                createNotebookInfo(boxInfo);
                GtkWidget *boxConex = gtk_box_new (GTK_ORIENTATION_VERTICAL,4);
                GtkWidget * lbConex = gtk_label_new (titleConex);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxConex,lbConex);
                g_signal_connect(GTK_NOTEBOOK (notebookMain), "switch-page", G_CALLBACK(conex_switchPage), this);
                createNotebookConexion(boxConex);
                GtkWidget * lbDowns = gtk_label_new (titleDowns);
                boxDowns = gtk_box_new (GTK_ORIENTATION_VERTICAL,1);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxDowns,lbDowns);
                downsTree =  new TreeView(boxDowns,&(config->downloads),this);
                GtkWidget * lbSels = gtk_label_new (titleSelects);
                boxSelects = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxSelects,lbSels);
                selectsTree =  new TreeView(boxSelects,&(config->selects),this);
                gtk_box_pack_start(GTK_BOX(vboxMain), notebookMain, FALSE, FALSE,0);
                
                return true;
        }
        void  Application::init(int*   argc, char **argv[])
        {
			gtk_init (argc, argv);
			if((*argc) == 1)
			{
				originFilename = "";
			}
			else if((*argc) > 1)
			{
				originFilename = (*argv)[1];
				std::cout << "File " << originFilename << std::endl;
			}
        }
                
                
	void Application::create()
	{
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		vboxMain = gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
		createWindow();        
                                
		createHeader();
                
		gtk_widget_show_all(window);  
		if(originFilename.size() > 0)documen_open(this,originFilename);
		gtk_main ();     
	}

         const char*  Application::titleInfo = "Información";
         const char*  Application::titleConex = "Conexión";
         const char*  Application::titleDowns = "Descargas";
         const char*  Application::titleSelects = "Selecciones";
        const char* TreeView::strNewFunct = "Agregar Funcion";
        char* Application::filename = NULL;
        std::string Application::nameApp = "APIDB";
}
}
