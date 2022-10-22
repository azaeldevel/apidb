
#include "driver.hpp"



namespace octetos
{
namespace apidb
{


InputMaria::InputMaria(octetos::db::Connector& c) : InputMM(c)
{
	
}
InputMaria::InputMaria(const octetos::db::Datconnect& d) : InputMM(d)
{
	conn = new octetos::db::maria::Connector;
	conn->connect(d);	
}
void InputMaria::read(const char* s)
{
	listing_tables(s);
}

}
}
