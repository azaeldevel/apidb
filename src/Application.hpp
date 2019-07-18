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
        private:
                void createWindow();
                void createToolbar();
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                void createNotebookDownloasAddTable();
                void createNotebookDownloas();
                void createNotebook();
                
                static void toolbar_chooseDirectory (GtkWidget *widget, gpointer   data);
                void loadConfig();
        private:
                GtkWidget *window;
                GtkWidget *vboxMain;
                GtkWidget *toolbar;
                GtkWidget *boxDowns;
                GtkWidget *btAddTable;
                std::vector<GtkWidget*> tables;
                //seccion de Informacion
                GtkWidget *inName;
                GtkWidget *inVer;
                GtkWidget *inInL;
                GtkWidget *inOutL;
                GtkWidget *inPkL;
                GtkWidget *inCmpl;
                //
                static char *filename;
                static octetos::apidb::ConfigureProject config;
                static Application* app;
        };
}
}

#endif
