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

#include "Application.hpp"

namespace octetos
{
namespace apidb
{
 
 
	std::string CaptureParameter::getSelectParam() const
	{
                return strParameter;
	}
	bool CaptureParameter::show()
	{
		gtk_widget_show_all(dialog);
		gint response = gtk_dialog_run(GTK_DIALOG(dialog));
		if(response == GTK_RESPONSE_OK)
		{
                        strParameter = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(cmbAddParameter));
                        gtk_widget_destroy(dialog);
                        return true;
		}
		else
		{
                        gtk_widget_destroy(dialog);
                        return false;
		}
                
		return false;
	}
	CaptureParameter::CaptureParameter(const Driver* d,const char* table) : driver(d)
	{
		dialog = gtk_dialog_new_with_buttons ("Captura de Parametro.", NULL, GTK_DIALOG_MODAL,  "gtk-ok", GTK_RESPONSE_OK, NULL);                
		//g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response),widget);
		content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
		label = gtk_label_new ("Seleccione la tabla para Agregar");
		gtk_container_add (GTK_CONTAINER (content_area), label);
		cmbAddParameter = gtk_combo_box_text_new ();
		gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,0,"selecione","Selecione..."); 
		if(driver != NULL)
		{ 
			int i = 0;
			std::list<std::string> list;
			driver->getFiledsName(list,table);
			for(std::list<std::string>::iterator it = list.begin(); it != list.end(); it++,i++)
			{
				gtk_combo_box_text_insert((GtkComboBoxText*)cmbAddParameter,i,(*it).c_str(),(*it).c_str()); 
			}
		}
		gtk_combo_box_set_active((GtkComboBox*)cmbAddParameter,0);
		gtk_container_add (GTK_CONTAINER (content_area), cmbAddParameter);
	}

}
}
