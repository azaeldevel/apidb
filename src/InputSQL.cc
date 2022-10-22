
#include "Inputs.hh"



namespace oct::api
{

const char* InputSQL::schema_name = "INFORMATION_SCHEMA";

InputSQL::InputSQL(octetos::db::Connector& c) : conn(&c),conn_free(false)
{
	
}
InputSQL::InputSQL(const octetos::db::Datconnect& d) : conn_free(true)
{
	if(not d.getDatabase().compare(schema_name))
	{
			throw octetos::core::Exception("Deve darse una conexion con el schema 'INFORMATION_SCHEMA'.",__FILE__,__LINE__);
	}
	
}
InputSQL::~InputSQL()
{
	if(conn_free) if(conn) delete conn;
	
}
void InputSQL::read(const char* s)
{
	listing_tables(s);
}
void InputSQL::listing_tables(const char* s)
{
	std::string sql = "SELECT TABLE_NAME FROM TABLES WHERE TABLE_SCHEMA = '";
	sql += s;
	sql += "';";
	//std::string sql = "SHOW TABLES;";
	//std::cout << "sql : " << sql << "\n";
	octetos::db::maria::Datresult dt;
	bool flag = conn->execute(sql,dt);
	if(flag)
	{
		std::cout << "Consulta competada\n";
		while (dt.nextRow())
		{
			std::cout << dt.getString(0) << "\n";
		}
	}
	else
	{
		std::cout << "Consulta fallida\n";
	}
}

}
