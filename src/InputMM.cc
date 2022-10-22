
#include "Inputs.hh"



namespace oct::api
{


InputMM::InputMM(octetos::db::Connector& c) : InputSQL(c)
{
	
}
InputMM::InputMM(const octetos::db::Datconnect& d) : InputSQL(d)
{
	
}
void InputMM::read(const char* space)
{
	
}

}

