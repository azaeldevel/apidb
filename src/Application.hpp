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
                
        public:                
                void CreateFill(std::vector<ConfigureProject::Table>&);
                GtkWidget *CreateView(std::vector<ConfigureProject::Table>&);
                void Tree(GtkWidget *,std::vector<ConfigureProject::Table>&);
        };
        class Application
        {
        public:
                void  init(int*   argc, char **argv[]);                
                void create();
                Application();
        private:
                enum Options
                {
                        RUN_DOWNLOAD,
                        RUN_SELECT
                };
                
                void createWindow();
                void createToolbar();
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                void createNotebookDownloasAddTable();
                void createNotebookDownloas();
                void createNotebookDownloasTree(Options option);
                void createNotebookSelects();
                void createNotebook();
                void createNotebookTablesSecc(GtkWidget *box,std::vector<ConfigureProject::Table>& list);
                void createNotebookDownloasTree_CreateFill(Options option);
                //GtkTreeModel *create_and_fill_model(void);
                GtkWidget *createNotebookDownloasTree_CreateView(Options option);
                
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
                GtkWidget *boxDowns;
                GtkWidget *boxDownsTables;
                GtkWidget *inLoc;
                GtkWidget *inPort;
                GtkWidget *inDB;
                GtkWidget *inUser;
                GtkWidget *inPw;
                //Descargas
                //GtkWidget *cmbAddTable;
                GtkTreeIter downsToplevel, downsTable,downsFuntion;
                GtkTreeStore *downsTreestore;
                GtkTreeModel *downsModel;
                GtkWidget *downsView;
                TreeView* downsTree;
                
                // Selects                
                GtkWidget *boxSelects;
                GtkWidget *boxSelectsTables;
                GtkTreeIter selectsToplevel, selectsTable,selectsFuntion;
                GtkTreeStore *selectsTreestore;
                GtkTreeModel *selectsModel;
                GtkWidget *selectsView;
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
