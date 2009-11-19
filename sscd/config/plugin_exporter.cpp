#ifndef SSCDCONFIGURATION_EXPORTER
#define SSCDCONFIGURATION_EXPORTER

#include "sscduserconfiguration.h"
#include "sscdnetworkconfiguration.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SSCDSettingsFactory, 
			registerPlugin< SSCDUserConfiguration >("SSCDUserConfiguration"); 
			registerPlugin< SSCDNetworkConfiguration >("SSCDNetworkConfiguration");
		)

K_EXPORT_PLUGIN( SSCDSettingsFactory("sscd"); )

#endif
