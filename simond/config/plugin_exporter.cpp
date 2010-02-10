#ifndef SIMONDCONFIGURATION_EXPORTER
#define SIMONDCONFIGURATION_EXPORTER

#include "simonduserconfiguration.h"
#include "simondnetworkconfiguration.h"
#include <kgenericfactory.h>
#include <KLocalizedString>
#include <simonuicomponents/multikcmview.h>

class SimondConfigMultiKCMView : public MultiKCMView
{
public:
	SimondConfigMultiKCMView(QWidget* parent, const QVariantList& args=QVariantList()) :
		MultiKCMView(parent, args)
	{
		registerModule(new SimondUserConfiguration(parent, args), KIcon("user-properties"), i18n("User"));
		registerModule(new SimondNetworkConfiguration(parent, args), KIcon("network-disconnect"), i18n("Network"));
	}
};

K_PLUGIN_FACTORY( SimondSettingsFactory, 
			registerPlugin< SimondConfigMultiKCMView >();
			//registerPlugin< SimondUserConfiguration >("SimondUserConfiguration"); 
			//registerPlugin< SimondNetworkConfiguration >("SimondNetworkConfiguration");
		)

K_EXPORT_PLUGIN( SimondSettingsFactory("simond"); )

#endif
