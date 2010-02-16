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


/**
 * Initializes the configuration. This will initialize the KAboutData object #about with the given values.
 *
 * \param parent The parent scenario
 * \param internalName The internal (not translated) name of the plugin
 * \param name The name to present to the user
 * \param version The current version of the plugin
 * \param desc Short description of the plugin
 * \param iconName The icon of your plugin
 * \param componentData Componentdata object
 * \param args Optional arguments
 */
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


/**
 * Conveniance method: emits changed(true)
 */
void CommandConfiguration::slotChanged()
{
	emit changed(true);
}

/**
 * Destructor: Deletes the about data object
 */
CommandConfiguration::~CommandConfiguration()
{
	delete about;
}
