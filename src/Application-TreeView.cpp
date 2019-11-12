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

#include <map>

#include "Application.hpp"
#include "Errors.hpp"
#include "ConfigureProject.hpp"


namespace octetos
{
namespace apidb
{ 
    gboolean TreeView::delete_keypress (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
    {
        TreeView* treeview = (TreeView*)user_data;
        if (event->keyval == GDK_KEY_KP_Delete | event->keyval == GDK_KEY_Delete )
        {
            std::cout << "Delete event \n";
            if(treeview->activeDelete)
            {
                treeview->tbDelete->erase(treeview->itDelete);
                treeview->activeDelete = false;
                treeview->tbDelete = NULL;
                treeview->fill();
                treeview->getApplication()->setSaved(false);
            }
        }  
        
        return TRUE;
    }
    bool TreeView::removeFunction(char* path_gtk, std::map<const char*,octetos::apidb::ConfigureProject::Table*,symbols::cmp_str>* lst, TreeView* treeview)
    {
        const char* vPath[] = {"","","",NULL};
        const char delim[] = ":"; 
        char* path = path_gtk;
        int i = -1;
        do
        {
            i++;
            vPath[i] = strtok(path, delim);
            path = NULL;
            std::cout << "i = " << i << ", ";
            if(vPath[i] != NULL) std::cout << vPath[i] << "\n";
        }
        while(vPath[i] != NULL);
        std::cout << "\n";
        std::map<const char*,octetos::apidb::ConfigureProject::Table*,symbols::cmp_str>::iterator it = lst->begin();
        std::advance(it,std::atoi(vPath[1]));
        treeview->tbDelete = it->second;
        std::cout << "Class:" << treeview->tbDelete->getName() << "\n";
        std::map<const char*, const octetos::apidb::ConfigureProject::Function*,symbols::cmp_str>::iterator itF = treeview->tbDelete->begin();
        std::advance(itF,std::atoi(vPath[2]));
        treeview->itDelete = itF;
        treeview->activeDelete = true;
        //const octetos::apidb::ConfigureProject::Function* fn = itF->second;
        //std::cout << "Function:" << fn->getName() << "\n";
        //cl->erase(itF);
        return true;
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
            case 'F':
            {
                if(removeFunction((char*)name,wgTree->list,wgTree))
                {
                    wgTree->getApplication()->setSaved(false);
                }
                break;
            }
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
                    //std::cout << "Tabla '" << strTable <<  "' encontrada para funcion " <<  strFunction << std::endl;
                    itT->second->insert(std::make_pair(newF->getName().c_str(), newF));
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
                g_signal_connect(G_OBJECT(view), "key-press-event", G_CALLBACK(delete_keypress), this);
                
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
                
                activeDelete = false;
        }
        
        
}
}
