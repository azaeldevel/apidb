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
        
        class TreeView
        {
        private:                
                GtkTreeIter toplevel, table,funtion;
                GtkTreeStore *treestore;
                GtkTreeModel *model;
                GtkWidget *view;
                std::map<const char*,ConfigureProject::Table*>* list;
                Application* app;
                
                //static GtkTreeIter* actual;
                static const char* strNewFunct;
                
                static void row_activated(GtkTreeView       *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer  user_data);
                static char checkTypeNode(GtkTreeModel *model,GtkTreeIter* iter);
                static const char* getTableName(GtkTreeModel *model,GtkTreeIter* iter,std::map<const char*,ConfigureProject::Table*>*);                
                
        public:                
                void fill();
                GtkWidget* create();
                TreeView(GtkWidget *,std::map<const char*,ConfigureProject::Table*>*,Application*);
                Application* getApplication()const;
        };
        
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
                
                //Application* getApplication();
                
        private:                
                void createWindow();
                //void createToolbar();
                void createNotebookInfo(GtkWidget *boxInfo);
                void createNotebookConexion(GtkWidget *boxConex);
                void createNotebook();
                void createHeader();
                
                static void chooseDirectory (GtkWidget *widget, gpointer   data);
                static void downloads_addTable (GtkWidget *widget, gpointer   data);
                void loadConfig();       
                static void  document_close(GtkWidget *widget, gpointer data);
                //static void active_tab (GtkNotebook *notebook, GtkWidget   *page, guint page_num, gpointer user_data);
                //static void on_newtable(GtkWidget *widget, gpointer data);
                
        private:
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
                ConfigureProject* config;
                Driver* driver;
                //static Application* app;
                bool flagVisible;
                //static const char* selectedTab;
                static std::string nameApp;
                //
                bool isSaved;
                void setSaved(bool);
        };
}
}

#endif
