#ifndef PLUGIN_EXPORTER
#define PLUGIN_EXPORTER

#include "networksettings.h"
#include "synchronisationsettings.h"
#include <kgenericfactory.h>
#include <simonuicomponents/multikcmview.h>

class RecognitionConfigMultiKCMView : public MultiKCMView
{
public:
	RecognitionConfigMultiKCMView(QWidget* parent, const QVariantList& args=QVariantList()) :
		MultiKCMView(parent, args)
	{
		registerModule(new NetworkSettings(parent, args), KIcon("simond"), i18n("Server"));
		registerModule(new SynchronisationSettings(parent, args), KIcon("view-refresh"), i18n("Synchronisation"));
	}
};

K_PLUGIN_FACTORY( ServerSettingsFactory, 
			registerPlugin< RecognitionConfigMultiKCMView >(); 
//			registerPlugin< NetworkSettings >("NetworkSettings"); 
//			registerPlugin< SynchronisationSettings >("SynchronisationSettings"); 
		)
        
K_EXPORT_PLUGIN( ServerSettingsFactory("ServerSettings") )

#endif
