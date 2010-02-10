#ifndef SIMONMODELMANAGEMENTUI_EXPORTER
#define SIMONMODELMANAGEMENTUI_EXPORTER

#include "modelsettings.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SpeechModelManagementUiFactory, 
			registerPlugin< ModelSettings >();
		)

K_EXPORT_PLUGIN( SpeechModelManagementUiFactory("SpeechModelManagementUi"); )

#endif
