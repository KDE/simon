/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
    MultiKCMView(parent, args) {
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
