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
        class CaptureParameter
        {
        public:
                void show();
                const char* getSelectTable()const;
                CaptureParameter(const Driver*,GtkWidget* widget);
        private:
                GtkWidget *dialog;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *cmbAddTable;
                const Driver* driver;
                const char* table;
        };
        class CaptureTable
        {
        public:
                void show();
                const char* getSelectTable()const;
                CaptureTable(const Driver*,GtkWidget* widget);
        private:
                GtkWidget *dialog;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *cmbAddTable;
                const Driver* driver;
                const char* table;
        };
        
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
                static void active_tab (GtkNotebook *notebook, GtkWidget   *page, guint page_num, gpointer user_data);
                static void on_newtable(GtkWidget *widget, gpointer data);
                
        private:
                GtkWidget *window;
                GtkWidget *vboxMain;
                GtkWidget *toolbar;
                std::vector<GtkWidget*> tables;
                //seccion de Informacion
                static const char* titleInfo;
                GtkWidget *inName;
                GtkWidget *inVer;
                GtkWidget *inInL;
                GtkWidget *inOutL;
                GtkWidget *inPkL;
                GtkWidget *inCmpl;
                //Conexion
                static const char* titleConex;
                GtkWidget *inLoc;
                GtkWidget *inPort;
                GtkWidget *inDB;
                GtkWidget *inUser;
                GtkWidget *inPw;
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
                static ConfigureProject config;
                static Driver* driver;
                static Application* app;
                static bool flagVisible;
                static const char* selectedTab;
                //
        };
}
}

#endif
