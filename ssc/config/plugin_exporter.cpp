#ifndef SSCCONFIGURATION_EXPORTER
#define SSCCONFIGURATION_EXPORTER

#include "sscconfiguration.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SSCSettingsFactory, 
			registerPlugin< SSCConfiguration >(); 
		)

K_EXPORT_PLUGIN( SSCSettingsFactory("ssc"); )

#endif
