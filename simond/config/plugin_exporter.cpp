#ifndef SIMONDCONFIGURATION_EXPORTER
#define SIMONDCONFIGURATION_EXPORTER

#include "simonduserconfiguration.h"
#include "simondnetworkconfiguration.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SimondConfigurationFactory, 
			registerPlugin< SimondUserConfiguration >("SimondUserConfiguration"); 
			registerPlugin< SimondNetworkConfiguration >("SimondNetworkConfiguration");
		)

K_EXPORT_PLUGIN( SimondConfigurationFactory("SimondConfiguration"); )

#endif
