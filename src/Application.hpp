#ifndef OCTETOS_APIDB_APPLICATION
#define OCTETOS_APIDB_APPLICATION

#include <gtk/gtk.h>
#include <vector>


#include "common.hpp"


namespace octetos
{
namespace apidb
{
        class TreeView
        {
        private:                
                GtkTreeIter toplevel, table,funtion;
                GtkTreeStore *treestore;
                GtkTreeModel *model;
                GtkWidget *view;
                std::vector<ConfigureProject::Table>& list;
                
        public:                
                void fill();
                GtkWidget* create();
                TreeView(GtkWidget *,std::vector<ConfigureProject::Table>&);
        };
        
        class Application
        {
        public:
                void  init(int*   argc, char **argv[]);                
                void create();
                Application();
        private:                
                void createWindow();
                void createToolbar();
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                void createNotebook();
                
                static void toolbar_chooseDirectory (GtkWidget *widget, gpointer   data);
                static void downloads_addTable (GtkWidget *widget, gpointer   data);
                void loadConfig();                
                
        private:
                GtkWidget *window;
                GtkWidget *vboxMain;
                GtkWidget *toolbar;
                std::vector<GtkWidget*> tables;
                //seccion de Informacion
                GtkWidget *inName;
                GtkWidget *inVer;
                GtkWidget *inInL;
                GtkWidget *inOutL;
                GtkWidget *inPkL;
                GtkWidget *inCmpl;
                //Conexion
                GtkWidget *inLoc;
                GtkWidget *inPort;
                GtkWidget *inDB;
                GtkWidget *inUser;
                GtkWidget *inPw;
                //Descargas
                GtkWidget *boxDowns;
                TreeView* downsTree;
                
                // Selects                
                GtkWidget* boxSelects;
                TreeView* selectsTree;
                //
                static char *filename;
                static octetos::apidb::ConfigureProject config;
                static Application* app;
                //
        };
}
}

#endif
