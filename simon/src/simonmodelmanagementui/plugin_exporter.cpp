#ifndef SIMONMODELMANAGEMENTUI_EXPORTER
#define SIMONMODELMANAGEMENTUI_EXPORTER

#include "internetextensionsettings.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SpeechModelManagementUiFactory, 
			registerPlugin< InternetExtensionSettings >("InternetExtensionSettings");
		)

K_EXPORT_PLUGIN( SpeechModelManagementUiFactory("SpeechModelManagementUi"); )

#endif