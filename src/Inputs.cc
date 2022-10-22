
#include "Inputs.hh"



namespace oct::api
{

const char* DataSourceMaria::engine_name()const
{
	return "maria";
}






const char* DataSourcePostgreSQL::engine_name()const
{
	return "postgresql";
}

}
