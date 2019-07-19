#ifndef OCTETOS_APIDB_APPLICATION
#define OCTETOS_APIDB_APPLICATION

#include <gtk/gtk.h>
#include <vector>


#include "common.hpp"


namespace octetos
{
namespace apidb
{
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
                void createNotebookDownloasAddTable();
                void createNotebookDownloas();
                void createNotebookSelects();
                void createNotebook();
                void createNotebookTablesSecc(GtkWidget *box,std::vector<ConfigureProject::Table>& list);
                
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
                GtkWidget *cmbAddTable;
                // Selects                
                GtkWidget *boxSelects;
                GtkWidget *boxSelectsTables;
                //
                static char *filename;
                static octetos::apidb::ConfigureProject config;
                static Application* app;
        };
}
}

#endif
