/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "commandconfiguration.h"
#include <KAboutData>
#include <KLocalizedString>
#include <QString>
#include <KSharedConfig>
#include <KComponentData>
#include <QVariantList>

CommandConfiguration::CommandConfiguration(Scenario *parent, const QByteArray& internalName, const KLocalizedString& name, 
					    const QByteArray& version, const KLocalizedString& desc,
					    const QString& iconName, const KComponentData& componentData,
					    const QVariantList &args)
			: QWidget(), ScenarioObject(parent)
{
	Q_UNUSED(args);

	about = new KAboutData(internalName, "", name,
				version, desc, KAboutData::License_GPL);
	about->setProgramIconName(iconName);
}


void CommandConfiguration::slotChanged()
{
	emit changed(true);
}

CommandConfiguration::~CommandConfiguration()
{
	delete about;
}
