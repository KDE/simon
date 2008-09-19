#include "sync_factory.h"
#include "sync_filewriter.h"
#include "sync_networkwriter.h"

Writer* Factory::getWriter(Configuration *config)
{
	if( config->getType() == 1 )
	{
		return new FileWriter(config);
	}
	if( config->getType() == 3)
	{
		return new NetworkWriter(config);
	}
	// TODO : Weitere if - else hier einfuegen...
	return new Writer(config);
}
