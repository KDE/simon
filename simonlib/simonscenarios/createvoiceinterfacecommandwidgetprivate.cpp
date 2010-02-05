/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "createvoiceinterfacecommandwidgetprivate.h"
#include "commandmanager.h"
#include "voiceinterfacecommand.h"
#include <QHash>

Command* CreateVoiceInterfaceCommandWidgetPrivate::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
	int currentActionIndex = ui.cbAction->currentIndex();
	if (currentActionIndex == -1) return NULL;

	QString id = ui.cbAction->itemData(currentActionIndex).toString();
//	QString actionName = ui.cbAction->currentText();

	return new VoiceInterfaceCommand(m_manager, name, iconSrc, description,
			id, ui.leVisibleTrigger->text());
}


bool CreateVoiceInterfaceCommandWidgetPrivate::init(Command* command)
{
	VoiceInterfaceCommand *c = dynamic_cast<VoiceInterfaceCommand*>(command);
	if (!c) return false;

	ui.cbAction->clear();

	QHash<QString, QString> voiceInterfaceActionNames = m_manager->getVoiceInterfaceActionNames();
	QStringList ids = voiceInterfaceActionNames.keys();
	foreach (const QString& id, ids)
		ui.cbAction->addItem(voiceInterfaceActionNames.value(id), id);

	int selectedActionIndex = ui.cbAction->findData(c->id());
	kDebug() << c->id() << selectedActionIndex;
	ui.cbAction->setCurrentIndex(selectedActionIndex);

	ui.leVisibleTrigger->setText(c->visibleTrigger());
	return true;
}

bool CreateVoiceInterfaceCommandWidgetPrivate::isComplete()
{
	return ((ui.cbAction->currentIndex() != -1) && (!(ui.leVisibleTrigger->text().isEmpty())));
}



//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


