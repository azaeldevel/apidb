


#include "v6.hh"
#include <Inputs.hh>

#ifdef APIDB_MYSQL
static octetos::db::mysql::Datconnect mysqlSource("localhost",3306,"muposys-0-alpha","muposys","123456");
#endif
#ifdef APIDB_POSTGRESQL
static octetos::db::postgresql::Datconnect postgresqlSource("localhost",5432,"sysapp_alpha","sysapp","123456"); 
#endif
#ifdef APIDB_MARIADB
static octetos::db::maria::Datconnect mariaSource("localhost",3306,"muposys-0-alpha","muposys","123456");
//static oct::api::DataSourceMaria dsMaria {"localhost","muposys","123456","muposys-0-alpha",3306,(const char*)NULL,0};
#endif

int v6_init(void)
{
	return 0;
}
int v6_clean(void)
{
	return 0;
}
void v6_develop()
{	
	octetos::db::maria::Datconnect dat_conn(mariaSource);
	dat_conn.setDatabase(oct::api::InputMaria::schema_name);
	octetos::db::maria::Connector conn;
	bool conn_conected = conn.connect(dat_conn);
	if(not conn_conected)
	{
		std::cout << "Falló la conexion al servidor : Linea " << __LINE__ << "\n";
		return;
	}
	CU_ASSERT(conn_conected);
	
	oct::api::InputMaria input(conn);
	try
	{
		input.read(mariaSource.getDatabase().c_str());
	}
	catch(const std::exception& e)
	{
		CU_ASSERT(false);
		std::cout << "Falló : " << __LINE__ << "\n\t" << e.what() << "\n";
	}
	
	conn.close();
}
