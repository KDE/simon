#ifndef PLUGIN_EXPORTER
#define PLUGIN_EXPORTER

#include "networksettings.h"
#include "synchronisationsettings.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( ServerSettingsFactory, 
			registerPlugin< NetworkSettings >("NetworkSettings"); 
			registerPlugin< SynchronisationSettings >("SynchronisationSettings"); 
		)
        
K_EXPORT_PLUGIN( ServerSettingsFactory("ServerSettings") )

#endif
