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
#include <dlfcn.h>


#include "Application.hpp"
#include "Errors.hpp"
#include "apidb.hpp"
#include "versionInfo.h"


namespace octetos
{
namespace apidb
{

    Application* TreeView::getApplication()const
    {
        return app;
    }
	bool Application::downConf()
	{
        //std::cout << "downConf : Step 1\n";
        if(config == NULL)
        {
                GtkWidget *msg = gtk_message_dialog_new (NULL,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_CLOSE,
                    "No se inicializado la estructura de Configuracion",
                    "Error", g_strerror (errno));
                    gtk_dialog_run (GTK_DIALOG (msg));       
                    gtk_widget_destroy (msg);                          
                    return false;           
            
        }
        
        if(isSaved and !isOpen) return true;
        //std::cout << "downConf : Step 2\n";
                
        
        if(inInLEdited)
        {
            int innIn = gtk_combo_box_get_active(GTK_COMBO_BOX(inInL));
            if(innIn == -1)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Selecione un lenguajde de entrada(MariaDB por ejemplo) en la sección de información","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));                                  
                gtk_widget_destroy (msg);
                return false;
            }
            const gchar* gcinl = gtk_combo_box_get_active_id(GTK_COMBO_BOX(inInL));
            //std::cout << "In Language : " << gcinl << "\n";
            config->setInputLenguaje(getInputLenguaje(gcinl));
        }
        //std::cout << "downConf : Step 3\n";
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
        
        //std::cout << "downConf : Step 4\n";
		
		
        if(inOutLEdited)
        {
            int intout = gtk_combo_box_get_active(GTK_COMBO_BOX(inOutL));
            if(intout == 0)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Selecione un lenguajde de salida(C++ por ejemplo) en la sección de información","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
            }
            config->outputLenguaje =  getOutputLenguajes(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inOutL)));
        }
        //std::cout << "downConf : Step 5\n";
        if(inVerEdited)
        {
            std::string verstr = gtk_entry_get_text(GTK_ENTRY(inVer));
            if(!config->versionResult.set(verstr))
            {                                       
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Fallo la lectura del la version, el formato aceptable es 'x.y.z'","Error", g_strerror (errno));
                                gtk_dialog_run (GTK_DIALOG (msg));  
                                gtk_widget_destroy (msg);
                                return false;
            }                                
        }
        //std::cout << "downConf : Step 6\n";
        if(inPkLEdited)
        {
            int intpkl = gtk_combo_box_get_active(GTK_COMBO_BOX(inPkL));
            if(intpkl == 0)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Selecione un opción de empaquetado en la seccion de información.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
                                
            }
            config->packing = getPackingLenguajes(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inPkL)));
        }
        //std::cout << "downConf : Step 7\n";
        if(inCmplEdited)
        {
            int intcmpl = gtk_combo_box_get_active(GTK_COMBO_BOX(inCmpl));
            if(intcmpl == 0)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Selecione un opción de empaquetado en la seccion de información.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
                                
            }
            config->compiled = getCompiled(gtk_combo_box_get_active_id(GTK_COMBO_BOX(inCmpl)));
        }
        //std::cout << "downConf : Step 8\n";
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
		
        //std::cout << "downConf : Step 9\n";
		
	if(inExeEdited)
        {
            std::string exe = gtk_entry_get_text(GTK_ENTRY(inExe));
            config->executable_target = exe;
        }
               
        /*if( config->newDatConnection() != NULL)
        {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Fallo al crear la estructura de Configuracion.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
        }*/
        
        //std::cout << "downConf : Step 10\n";
        if(config->getDatconnection() == NULL)
        {
        	//std::cout << "downConf : Step 10.1.1\n";
        	if(config->newDatConnection())
        	{
        		//std::cout << "downConf : new conection created\n";
        	}
        	else
        	{
        		//::cout << "downConf : fail new conection created\n";
        	}
        }
        //std::cout << "downConf : Step 10.2\n";
        if(core::Error::check())
        {
        	return false;
        }
        //std::cout << "downConf : Step 10.3\n";
        if(inLocEdited)
        {
            std::string loc = gtk_entry_get_text(GTK_ENTRY(inLoc));
            if(loc.empty())
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Indique la direcion del host o sitio web en la seccion de Conexión.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
            }
            config->getDatconnection()->setHost(loc);
        }
        
        //std::cout << "downConf : Step 11\n";
        if(inPortEdited)
        {
            std::string strport = gtk_entry_get_text(GTK_ENTRY(inPort));
            if(strport.empty())
            {
                GtkWidget *msg = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Indique el número de puerto.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
            }
            unsigned int intport = std::stoi(strport.c_str());
            if(intport < 1)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"El número de puesto deve er un entero positivo.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));  
                gtk_widget_destroy (msg);
                return false;
            }
            config->getDatconnection()->setPort(intport);
        }
        //std::cout << "downConf : Step 11\n";
        if(inDBEdited) 
        {
            std::string strdb = gtk_entry_get_text(GTK_ENTRY(inDB));
            if(strdb.empty())
            {                                
                GtkWidget *msg = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Indique una base de datos.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));   
                gtk_widget_destroy (msg);
                return false;
            }
            config->getDatconnection()->setDatabase(strdb);
        }
        //std::cout << "downConf : Step 12\n";
        if(inUserEdited)
        {
            std::string struser = gtk_entry_get_text(GTK_ENTRY(inUser));
            if(struser.empty())
            {                                
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Indique un nombre de usuario para la Base de Datos.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));   
                gtk_widget_destroy (msg);
                return false;
            }
            config->getDatconnection()->setUser(struser);
        }
        //std::cout << "downConf : Step 13\n";
        if(inPwEdited)
        {
            std::string strpw = gtk_entry_get_text(GTK_ENTRY(inPw));   
            if(strpw.empty())
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,"Dejo vacio el campo de contraseña.","Error", g_strerror (errno));
                gtk_dialog_run (GTK_DIALOG (msg));    
                gtk_widget_destroy (msg);
            }
            config->getDatconnection()->setPassword(strpw);
        }
        //std::cout << "downConf : Step 14\n";
        if(inWConnNameEdited)
        {
            std::string str = gtk_entry_get_text(GTK_ENTRY(inWConnName));
            if(!str.empty() and str.compare("¿?") != 0)
            {
                config->writeDatconnect = gtk_entry_get_text(GTK_ENTRY(inWConnName)); 
            }
        }
                
		return true;
	}
	
	void Application::build(GtkWidget *widget, gpointer data) 
	{
        //std::cout << "build : Step 1\n";
        Application* app = (Application*)data;
        if(!app->isSaved)
        {
            GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Deve guardar el documento antes de continuar la operacion.","Al construir el proyecto", g_strerror (errno));
            gtk_dialog_run (GTK_DIALOG (msg)); 
            gtk_widget_destroy (msg);
            return;
        }
        //std::cout << "build : Step 2\n";
                
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
                                if(core::Error::check())
                                {
                                                msgstr = (&core::Error::get())->what();
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
            GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str(), msgstr.c_str(), g_strerror (errno));
            gtk_dialog_run (GTK_DIALOG (msg));    
            gtk_widget_destroy (msg);
            return;
        }
        
        //std::cout << "build : Step 3\n";
        
        bool flagDriver = false;
        try
        {
            //std::cout << "build : Step 3.1\n";
            if(app->driver != NULL) 
            {
                delete (app->driver);
                app->driver = NULL;
            }
            //std::cout << "build : Step 3.2\n";
            app->driver = new Driver(*(app->config));
            //std::cout << "build : Step 3.3\n";
            Tracer tr(0);
            //std::cout << "build : Step 3.4\n";
            flagDriver = app->driver->driving(&tr);
            //std::cout << "build : Step 3.5\n";
        }
        catch(BuildException e)
        {
            GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,e.what());
            gtk_dialog_run (GTK_DIALOG (msg)); 
            gtk_widget_destroy (msg);
            return;
        }
        //std::cout << "build : Step 4\n";
		if(!flagDriver)
		{
			std::string msgstr = "";
			if(core::Error::check())
			{
				msgstr = (&core::Error::get())->what();
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
        //std::cout << "document_saveas : Step 1\n";
        Application* app = (Application*)data;
        
        //std::cout << "document_saveas : Step 1.1\n";
        
        if(app->config == NULL)
        {
        	//std::cout << "document_saveas : app->config is NULL\n";
            app->config = new ConfigureProject();                
        }
            
        //std::cout << "document_saveas : Step 1.2\n";
        
        if(!app->downConf())
        {
            std::string strmsg = "Falló la operacionde Guardar";
            GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,strmsg.c_str());
            gtk_dialog_run (GTK_DIALOG (msg)); 
            gtk_widget_destroy (msg);
            return;
        }
        //std::cout << "document_saveas : Step 2\n";
       
        GtkWidget *dialog;
        GtkFileChooser *chooser;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
        gint res;
        //std::cout << "document_saveas : Step 3\n";

        dialog = gtk_file_chooser_dialog_new ("Save File", NULL,action,"_Cancel",GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT, NULL);
        chooser = GTK_FILE_CHOOSER (dialog);
        gtk_file_chooser_set_current_name (chooser,(app->config->name + ".apidb").c_str());
        res = gtk_dialog_run (GTK_DIALOG (dialog));
        //std::cout << "document_saveas : Step 4\n";
        if (res == GTK_RESPONSE_ACCEPT)
        {
            char *filename;

            filename = gtk_file_chooser_get_filename (chooser);
            app->config->saveConfig(filename);
			app->originFilename = filename;
            //delete app->config;
            app->isOpen = true;
            app->isNew = false;
            app->setSaved(true);
            g_free (filename);
        }
        gtk_widget_destroy (dialog);                
    }
	void Application::document_save(GtkWidget *widget, gpointer data) 
	{
		//std::cout << "document_save:Step 1" << std::endl;
		Application* app = (Application*)data;
        
        if(app->config == NULL)
        {
        	//std::cout << "document_save: app->config is NULL" << std::endl;
            app->config = new ConfigureProject();                
        }
        
		//std::cout << "document_save:Step 2" << std::endl;   
		/*if(!app->isOpen || !app->isSaved)
        {//no esta abierto el proyecto.                                
            std::string msgstr;
            if(core::Error::check())
            {
					msgstr = core::Error::get().what();
            }
            else
            {
					msgstr = "No hay documento abierto o pendiente de guardar";
            }
            GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
            gtk_dialog_run (GTK_DIALOG (msg)); 
            gtk_widget_destroy (msg);
            return;
        }*/
        
		//std::cout << "document_save:Step 3" << std::endl;
        
		if(!app->originFilename.empty() and !app->isNew) //si fue cargado simplemete usa el mismo archivo
		{
			//std::cout << "Step 3.1" << std::endl;
			//escribir en la estura de configuracion del proyecto.
			if(!app->downConf())
			{
				std::string msgstr;
				if(core::Error::check())
				{
					msgstr = (&core::Error::get())->what();
				}
				else
				{
					msgstr = "Ocurrio un erro desconocido la operacion de guardar el archivo.";
				}
				GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
				gtk_dialog_run (GTK_DIALOG (msg)); 
				gtk_widget_destroy (msg);
				return;
			}
			//Guarda los datos en disco
			//std::cout << "Step 3.2" << std::endl;
			try
			{
				app->config->saveConfig(app->originFilename);
			}
			catch (std::exception e)
			{
				GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,e.what());
				gtk_dialog_run (GTK_DIALOG (msg)); 
				gtk_widget_destroy (msg);
				return;
			}
			//std::cout << "Step 2.4" << std::endl;                      
			app->isOpen = true;
			app->setSaved(true);
			//std::cout << "Step 2.5" << std::endl;
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
		//std::cout << "document_save:Step 4" << std::endl;
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
        
            if(app->config == NULL)
            {
                app->config = new ConfigureProject();                
            }
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
            if(handle) dlclose(handle); 
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
            inOutLEdited = false;
            inPkLEdited = false;
            inCmplEdited = false;
            isNew = false; 
            handle = NULL;
            //createConnector = NULL;
            createDatconnect = NULL;
        }   
        /**
         * 
         * 
         */
        void Application::loadConfig()
        {         
			std::cout << "Application::loadConfig : Step  1"<< std::endl;
			gtk_entry_set_text (GTK_ENTRY(inName),config->name.c_str());
			std::cout << "Application::loadConfig : Step  2"<< std::endl;
			gtk_entry_set_text (GTK_ENTRY(inVer),config->versionResult.toString().c_str());
			std::cout << "Application::loadConfig : Step  3"<< std::endl;
			gtk_combo_box_set_active(GTK_COMBO_BOX(inInL),(gint)config->getInputLenguaje());
			std::cout << "Application::loadConfig : Step  "<< std::endl;
			gtk_combo_box_set_active(GTK_COMBO_BOX(inOutL),(gint)config->outputLenguaje);
			std::cout << "Application::loadConfig : Step  4"<< std::endl;
			gtk_combo_box_set_active(GTK_COMBO_BOX(inPkL),(gint)config->packing);
			std::cout << "Application::loadConfig : Step  5"<< std::endl;
			gtk_combo_box_set_active(GTK_COMBO_BOX(inCmpl),(gint)config->compiled);
			std::cout << "Application::loadConfig : Step  "<< std::endl;
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (inFileChooserBuildDirectory),config->builDirectory.c_str());
			std::cout << "Application::loadConfig : Step  6"<< std::endl;
			gtk_combo_box_set_active(GTK_COMBO_BOX(inNameSpaceDetect),inNameSpaceDetect_comboxid(config->namespace_detect));
			std::cout << "Application::loadConfig : Step  7"<< std::endl;
			if(!config->writeDatconnect.empty() and config->writeDatconnect.compare("¿?") != 0) gtk_entry_set_text(GTK_ENTRY(inWConnName),config->writeDatconnect.c_str());
			std::cout << "Application::loadConfig : Step  8"<< std::endl;
			if(!config->executable_target.empty() and config->executable_target.compare("¿?") != 0)gtk_entry_set_text (GTK_ENTRY(inExe),config->executable_target.c_str());
			std::cout << "Application::loadConfig : Step  9"<< std::endl;
                
			gtk_entry_set_text(GTK_ENTRY(inLoc),config->getDatconnection()->getHost().c_str());
			std::cout << "Application::loadConfig : Step  10"<< std::endl;
			//std::cout << "Application::loadConfig : Step  10.1 : " << strport << std::endl;
			gtk_entry_set_text(GTK_ENTRY(inPort),std::to_string(config->getDatconnection()->getPort()).c_str());
			std::cout << "Application::loadConfig : Step  11"<< std::endl;
			gtk_entry_set_text(GTK_ENTRY(inDB),config->getDatconnection()->getDatabase().c_str());
			std::cout << "Application::loadConfig : Step  12"<< std::endl;
			gtk_entry_set_text(GTK_ENTRY(inUser),config->getDatconnection()->getUser().c_str());
			std::cout << "Application::loadConfig : Step  13"<< std::endl;
			gtk_entry_set_text(GTK_ENTRY(inPw),config->getDatconnection()->getPassword().c_str());
			std::cout << "Application::loadConfig : Step  14"<< std::endl;
                               
			std::cout << "Application::loadConfig : Step  15"<< std::endl;
			downsTree->fill();
                
			std::cout << "Application::loadConfig : Step  16"<< std::endl;
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
	void Application::documen_open(Application* app,const std::string& filename)
	{
		std::cout << "Application::documen_open: Step 1 "<< std::endl;
		if(app->config == NULL)
		{
			app->config = new ConfigureProject();
		}
		std::cout << "Application::documen_open : Step 2" << std::endl;
		try
		{
			std::cout << "Application::documen_open : Step 2.1" << std::endl;            
			std::cout << "Application::documen_open : Step 2.2" << std::endl;
			app->config->readConfig(filename);          
			std::cout << "Application::documen_open : Step 2.3" << std::endl;
			if(app->config->checkFailLoadDat())
			{
				app->originFilename = filename;
				app->createNotebook();          
                std::cout << "Application::documen_open : Step 2.4" << std::endl;
				app->loadConfig();          
                std::cout << "Application::documen_open : Step 2.5" << std::endl;
				app->setSaved(true);
				app->isOpen = true;
				app->isNew = false;
				gtk_widget_show_all(app->window);
				return;
			}
			else
			{
				std::string msgstr = "Erro desconocido en checkFailLoadDat.";
				GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,msgstr.c_str());
				gtk_dialog_run (GTK_DIALOG (msg));
				gtk_widget_destroy (msg);
			}			
		}
		catch(const std::exception& e)
		{
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE, e.what());
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);
			return;
		}
		std::cout << "Application::documen_open : Step 3" << std::endl;
		if(app->driver != NULL) 
		{
			delete (app->driver);
			app->driver = NULL;
		}

		std::cout << "Application::documen_open : Step 3.6" << std::endl;
		try
		{
            app->driver = new Driver(*(app->config));
        }
        catch(const std::exception& e)
		{
			std::string msgstr = e.what();			
            msgstr = "Erro desconocido en Driver.";
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,msgstr.c_str());
			gtk_dialog_run (GTK_DIALOG (msg));
			gtk_widget_destroy (msg);
			app->originFilename = filename;
			app->createNotebook();
			app->loadConfig();
			app->setSaved(true);
			app->isOpen = true;
			app->isNew = false;
			gtk_widget_show_all(app->window);
            return;
		}
		std::cout << "Application::documen_open : Step 4" << std::endl;
		if(!app->driver->analyze(NULL))
		{
			std::string msgstr = "";
			if(core::Error::check())
			{
				msgstr = (&core::Error::get())->what();
			}
			else
			{
				msgstr ="Fallo la lectura del archivo de proyecto";
			}
			GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,msgstr.c_str());
			gtk_dialog_run (GTK_DIALOG (msg)); 
			gtk_widget_destroy (msg);                                
		}
		std::cout << "Application::documen_open : Step 5" << std::endl;
		app->originFilename = filename;
		app->createNotebook();
		app->loadConfig();
		app->setSaved(true);
		app->isOpen = true;
		app->isNew = false;
		gtk_widget_show_all(app->window);
		std::cout << "Application::documen_open : Step 6" << std::endl;
	}
	void Application::document_open (GtkWidget *widget, gpointer   data)
	{
		Application* app = (Application*)data;
		std::string filename;
		
		GtkWidget *dialog = gtk_file_chooser_dialog_new("Seleccionar Proyecto",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);   
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			char* tmpfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			filename = tmpfilename;
			gtk_widget_destroy (dialog);
			g_free (tmpfilename);
		}
		gtk_widget_destroy (dialog);
		
				
		documen_open(app,filename);
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
        gboolean Application::inExe_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;
                 
                if(app->config != NULL)
                {
                                app->inExeEdited = true;
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
                app->inPortEdited = true;
                app->inDBEdited = true;
                app->inUserEdited = true;
                app->inPwEdited = true;
                /*if(app->config->newDatConnection())
                {
                	//error
                }*/
                app->setSaved(false);
            }
            else if(!app->isOpen)
            {
                GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Deve crear un proyecto antes de captura informacion.","Error", g_strerror (errno));
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
			//std::cout <<"Active " << gtk_combo_box_get_active(widget) << std::endl;
			//std::cout <<"Active id " << gtk_combo_box_get_active_id(widget) << std::endl;
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
		std::string namestr;
		
		gtk_combo_box_text_insert((GtkComboBoxText*)inInL,0,"selecione","Selecione..."); 
		gtk_combo_box_set_active((GtkComboBox*)inInL,0);
		
		namestr = getInputLenguaje(InputLenguajes::MySQL);	gtk_combo_box_text_insert((GtkComboBoxText*)inInL,InputLenguajes::MySQL,namestr.c_str(),namestr.c_str());
		namestr = getInputLenguaje(InputLenguajes::PostgreSQL);		gtk_combo_box_text_insert((GtkComboBoxText*)inInL,InputLenguajes::PostgreSQL,namestr.c_str(),namestr.c_str());	
		namestr = getInputLenguaje(InputLenguajes::MariaDB);	gtk_combo_box_text_insert((GtkComboBoxText*)inInL,InputLenguajes::MariaDB,namestr.c_str(),namestr.c_str());
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
                
                
                GtkWidget *boxExe = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                GtkWidget * lbExe = gtk_label_new ("Ejecutable:");
                gtk_box_pack_start(GTK_BOX(boxExe), lbExe, FALSE, FALSE,0); 
                inExe = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxExe), inExe, FALSE, FALSE,0);   
                gtk_box_pack_start(GTK_BOX(boxInfo), boxExe, FALSE, FALSE,0);
                gtk_widget_set_events(inName,GDK_KEY_PRESS_MASK);
                g_signal_connect(G_OBJECT(inExe), "key-press-event", G_CALLBACK(inExe_keypress), this);
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
			else
			{
				core::Error::write(core::Error("Opción de 'Espacio virtual detectado.'",ErrorCodes::APIDB_FAIL,__FILE__,__LINE__));
				return -1;
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
        gboolean Application::inWConnName_keypress (GtkWidget *widget,GdkEventKey  *event,gpointer   user_data)
        {
                Application* app = (Application*)user_data;           
				if(app->config != NULL)
				{                                
					if(app->isSaved)app->setSaved(false);
					app->inWConnNameEdited = true;
				}
				else if(!app->isOpen)
				{
					GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE, "Deve crear un proyecto antes de captura informacion.", "Error", g_strerror (errno));
					gtk_dialog_run (GTK_DIALOG (msg)); 
					gtk_widget_destroy (msg);
                }
                return FALSE;
        }
		void Application::wConn_OnClick (GtkToggleButton *toggle_button,  gpointer  user_data)
		{
			Application* app = ((Application*)user_data);
			gtk_entry_set_visibility (GTK_ENTRY(app->inWConnName),!gtk_toggle_button_get_active (toggle_button));			
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
                inPort = gtk_entry_new();
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
				
                boxWConn= gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
                inWConn = gtk_check_button_new_with_label("Generar estructura de Conexión:");
                gtk_entry_set_visibility(GTK_ENTRY(inWConn),FALSE);
                gtk_box_pack_start(GTK_BOX(boxWConn), inWConn, FALSE, FALSE,0);
                gtk_box_pack_start(GTK_BOX(boxConex), boxWConn, FALSE, FALSE,0);
                //g_signal_connect(G_OBJECT(inWConn), "toggled", G_CALLBACK(wConn_OnClick), this);
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (inWConn), TRUE);
                inWConnName = gtk_entry_new();
                gtk_box_pack_start(GTK_BOX(boxWConn), inWConnName, FALSE, FALSE,0);   
                g_signal_connect(G_OBJECT(inWConnName), "key-press-event", G_CALLBACK(inWConnName_keypress), this);
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
			//std::cout << "Jump 1" << std::endl;
			if(app->inLocEdited || app->inPortEdited || app->inDBEdited || app->inUserEdited || app->inPwEdited)
			{
				app->conexEdited = true;
				if(app->isSaved) app->setSaved(false);
				if(app->isNew)  app->downConf();
				//std::cout << "Jump 2" << std::endl;
				try
				{
					//std::cout << app->config->conectordb->getHost() << std::endl;
					//std::cout << app->config->conectordb->getDatabase() << std::endl;
					//std::cout << app->config->conectordb->getPort() << std::endl;
					//std::cout << app->config->conectordb->getPassword() << std::endl;
					/*if(!app->config->testConexion())
					{                                        
						GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Fallo la conexion a la Base de datos revise sus parametros de conexion.");
						gtk_dialog_run (GTK_DIALOG (msg)); 
						gtk_widget_destroy (msg);
						return;
					}*/
				}
				catch(octetos::db::SQLException e)
				{
					GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,e.what());
					gtk_dialog_run (GTK_DIALOG (msg)); 
					gtk_widget_destroy (msg);
					return;
				}
                                //std::cout << "Jump 3" << std::endl;
                                if(app->driver == NULL)
                                {
                                        app->driver = new Driver(*(app->config));
                                }
                                else
                                {
                                        delete (app->driver);
                                        app->driver = new Driver(*(app->config));
                                }
                                //std::cout << "Jump 4" << std::endl;
                                if(!app->driver->analyze(NULL))
                                {
                                        std::string msgstr = "";
                                        if(core::Error::check())
                                        {
                                                msgstr = (&core::Error::get())->what();
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
			else
			{
				/*if(!app->config->testConexion())
				{                                        
					GtkWidget *msg = gtk_message_dialog_new (NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Fallo la conexion a la Base de datos revise sus parametros de conexion.");
					gtk_dialog_run (GTK_DIALOG (msg)); 
					gtk_widget_destroy (msg);
					return;
				}*/
				
			}
		}
	}
        bool Application::createNotebook()
        {                
                if(config == NULL)
                {
                        std::string msg = "Deve cargar del proyecto. '";
                        core::Error::write(core::Error(msg,ErrorCodes::APPLICATION_GTK3_CONFIGPROJECT_NULL));
                        return false;
                }
                
                notebookMain = gtk_notebook_new();
                //g_signal_connect(G_OBJECT(notebookMain), "switch-page", G_CALLBACK(active_tab), window);
                GtkWidget *boxInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL,8);
                GtkWidget *lbInfo = gtk_label_new (titleInfo);
                gtk_notebook_append_page (GTK_NOTEBOOK (notebookMain),boxInfo,lbInfo);
                createNotebookInfo(boxInfo);
                boxConex = gtk_box_new (GTK_ORIENTATION_VERTICAL,4);
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
