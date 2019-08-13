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

#ifndef OCTETOS_APIDB_APPLICATION
#define OCTETOS_APIDB_APPLICATION

#include <gtk/gtk.h>
#include <vector>


#include "common.hpp"
#include "driver.hpp"


namespace octetos
{
namespace apidb
{
        class Application;
        
        
        /**
         * \private No es parte del API
         * */
        class CaptureParameter
        {
        public:
                bool show();
                const char* getSelectParam()const;
                CaptureParameter(const Driver*,const char* table);
                
        private:
                GtkWidget *dialog;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *cmbAddParameter;
                const Driver* driver;
                const char* strParameter;
        };
        
        /**
         * \private No es parte del API
         * */
        class CaptureFuntion
        {
        public:
                void show();
                std::string getNameFunction()const;
                CaptureFuntion(const Driver*,GtkTreeIter* gtkIt,const char* table);
                
        private:
                GtkWidget *dialog;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *inAddFunc;
                const Driver* driver;
                std::string strNameFunction;
        };
        
        /**
         * \private No es parte del API
         * */
        class CaptureTable
        {
        public:
                bool show();
                const char* getSelectTable()const;
                CaptureTable(const Driver*);
                
        private:
                GtkWidget *dialog;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *cmbAddTable;
                const Driver* driver;
                const char* table;
        };
        
        /**
         * \private No es parte del API
         * */
        class TreeView
        {
        private:                
                GtkTreeIter toplevel, table,funtion;
                GtkTreeStore *treestore;
                GtkTreeModel *model;
                GtkWidget *view;
                std::map<const char*,ConfigureProject::Table*,symbols::cmp_str>* list;
                Application* app;
                
                static const char* strNewFunct;
                
                static void row_activated(GtkTreeView       *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer  user_data);
                static char checkTypeNode(GtkTreeModel *model,GtkTreeIter* iter);
                static const char* getTableName(GtkTreeModel *model,GtkTreeIter* iter,std::map<const char*,ConfigureProject::Table*,symbols::cmp_str>*);                
                
        public:                
                void fill();
                GtkWidget* create();
                TreeView(GtkWidget *,std::map<const char*,ConfigureProject::Table*,symbols::cmp_str>*,Application*);
                Application* getApplication()const;
        };
        
        /**
         * \private No es parte del API
         * */
        class Application
        {
        public:
                void  init(int*   argc, char **argv[]);                
                void create();
                Application();
                ~Application();
                Driver* getDriver();
                ConfigureProject* getConfigure()const;
                
                TreeView* getDownloadTreeView();
                TreeView* getSelectTreeView();
                
                void setSaved(bool);
                
        private:                
                void createWindow();
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                bool createNotebook();
                void createHeader();
                
                static void downloads_addTable (GtkWidget *widget, gpointer   data);
                void loadConfig();       
                static void document_open (GtkWidget *widget, gpointer   data);
                static void  document_close(GtkWidget *widget, gpointer data);
                static void document_new(GtkWidget *widget, gpointer data);
                static void document_save(GtkWidget *widget, gpointer data);
                static void document_saveas(GtkWidget *widget, gpointer data);
                static gboolean inName_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static gboolean inVer_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static void inInL_changed (GtkComboBox *widget, gpointer     user_data);
                static void inOutL_changed (GtkComboBox *widget, gpointer     user_data);
                static void inPkL_changed (GtkComboBox *widget, gpointer     user_data);
                static void inCmpl_changed (GtkComboBox *widget, gpointer     user_data);
                static gboolean inLoc_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static gboolean inPort_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static gboolean inDB_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static gboolean inUser_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static gboolean inPw_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data);
                static void buildDirectory_fileset (GtkFileChooserButton *widget,gpointer user_data);
                static void build(GtkWidget *widget, gpointer data);
                static void show_about(GtkWidget *widget, gpointer data) ;
                static gboolean conex_lostfocus (GtkWidget *widget, GdkEvent  *event,  gpointer   user_data);
                static void conex_switchPage (GtkNotebook *notebook, GtkWidget   *page, guint page_num, gpointer     user_data);
                static void application_destroy (GtkWidget *object, gpointer   user_data);
                bool downConf();
                       
                GtkWidget *window;
                GtkWidget *vboxMain;
                GtkWidget *toolbar;
                GtkWidget *headerbar;
                GtkWidget * notebookMain;
                GtkWidget  *btOpen;
                GIcon  *icoOpen;
                GtkWidget  *imgOpen;
                GtkWidget  *btNew;
                GIcon  *icoNew;
                GtkWidget  *imgNew;
                GtkWidget  *boxHeader;
                GtkWidget  *btBuild;
                GtkWidget *sep1;
                GIcon  *icoBuild;
                GtkWidget  *imgBuild;
                GtkWidget  *btSave;
                GIcon  *icoSave;
                GtkWidget  *imgSave;
                GtkWidget *sep2;
                GtkWidget  *btClose;
                GIcon  *icoClose;
                GtkWidget  *imgClose;
                GtkWidget  *btQuit;
                GIcon  *icoQuit;
                GtkWidget *sep3;
                GtkWidget  *imgQuit;
                GtkWidget  *btAbout;
                GIcon  *icoAbout;
                GtkWidget  *imgAbout;
                GtkWidget *sep4;
                GtkWidget  *imgCloseDoc;
                GtkWidget  *btCloseDoc;
                GIcon  *icoCloseDoc;
                GtkWidget  *btSaveAs;
                GIcon  *icoSaveAs;
                GtkWidget  *imgSaveAs;
                
                std::vector<GtkWidget*> tables;
                //seccion de Informacion
                static const char* titleInfo;
                GtkWidget *inName;
                bool inNameEdited;
                GtkWidget *inVer;
                bool inVerEdited;
                GtkWidget *inInL;
                bool inInLEdited;
                GtkWidget *inOutL;
                bool inOutLEdited;
                GtkWidget *inPkL;
                bool inPkLEdited;
                GtkWidget *inCmpl;            
                bool inCmplEdited;
                GtkWidget *inFileChooserBuildDirectory;
                GtkWidget *dlgBuildDirectory;
                GtkFileChooser *chooser_BuilDirectory;
                //Conexion
                static const char* titleConex;
                bool conexEdited;
                GtkWidget *inLoc;
                bool inLocEdited;
                GtkWidget *inPort;
                bool inPortEdited;
                GtkWidget *inDB;
                bool inDBEdited;
                GtkWidget *inUser;
                bool inUserEdited;
                GtkWidget *inPw;
                bool inPwEdited;
                //Descargas
                static const char* titleDowns;
                GtkWidget *boxDowns;
                TreeView* downsTree;                
                // Selects                
                static const char* titleSelects;
                GtkWidget* boxSelects;
                TreeView* selectsTree;
                //
                static char *filename;
                ConfigureProject* config;
                Driver* driver;
                static std::string nameApp;
                //
                bool isSaved;
                bool isOpen;
                bool isNew;
                std::string originFilename;
                
        };
}
}

#endif
