/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
    MultiKCMView(parent, args) {
      registerModule(new NetworkSettings(parent, args), KIcon("simond"), i18n("Server"));
      registerModule(new SynchronisationSettings(parent, args), KIcon("view-refresh"), i18n("Synchronization"));
    }
};

K_PLUGIN_FACTORY( ServerSettingsFactory,
registerPlugin< RecognitionConfigMultiKCMView >();
//			registerPlugin< NetworkSettings >("NetworkSettings");
//			registerPlugin< SynchronisationSettings >("SynchronisationSettings");
)

K_EXPORT_PLUGIN( ServerSettingsFactory("ServerSettings") )
#endif
