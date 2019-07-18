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
                void createWindow(GtkWidget* vboxMain,GtkWidget *window);
                void createToolbar(GtkWidget* vboxMain,GtkWidget* toolbar);
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                void createNotebookDownloasAddTable();
                void createNotebookDownloas();
                void createNotebook(GtkWidget* vboxMain);
                
                static void toolbar_chooseDirectory (GtkWidget *widget, gpointer   data);
        private:
                GtkWidget *window;
                GtkWidget *vboxMain;
                GtkWidget *toolbar;
                GtkWidget *boxDowns;
                GtkWidget *btAddTable;
                std::vector<GtkWidget*> tables;
                //
                static char *filename;
                //static apidb::ConfigureProject config;
        };
}
}

#endif
