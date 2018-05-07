#include "clientdb.hpp"
#include "config.h"

namespace clientdb
{

	const char* getPakageName()
	{
		return PAKAGENAME;
	}
	toolkit::Version getPakageVersion()
	{
		return toolkit::Version(VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH,VERSION_STAGE);		
	}
}	


