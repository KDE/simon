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
#include "voiceinterfacecommandtemplate.h"
#include <QHash>
#include <KIcon>
#include <KMessageBox>

CreateVoiceInterfaceCommandWidgetPrivate::CreateVoiceInterfaceCommandWidgetPrivate(CommandManager *manager, QWidget *parent) : 
		CreateCommandWidget(manager, parent)
{
	ui.setupUi(this);

	ui.cbAction->clear();
	QList<VoiceInterfaceCommandTemplate*> voiceInterfaceCommandTemplates = manager->getVoiceInterfaceCommandTemplates();
	foreach (VoiceInterfaceCommandTemplate* tem, voiceInterfaceCommandTemplates)
		ui.cbAction->addItem(KIcon(tem->icon()), tem->actionName(), tem->id());

//	QHash<QString, QString> voiceInterfaceActionNames = m_manager->getVoiceInterfaceActionNames();
//	QStringList ids = voiceInterfaceActionNames.keys();
//	foreach (const QString& id, ids)
		//ui.cbAction->addItem(voiceInterfaceActionNames.value(id), id);

	connect(ui.cbAction, SIGNAL(currentIndexChanged(int)), this, SIGNAL(completeChanged()));
	connect(ui.pbFromTemplate, SIGNAL(clicked()), this, SLOT(initFromTemplate()));
}

Command* CreateVoiceInterfaceCommandWidgetPrivate::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
	kDebug() << "Creating command";
	VoiceInterfaceCommandTemplate* tem = getCurrentTemplate();
	if (!tem) return NULL;

	VoiceInterfaceCommand *voiceInterfaceCommand = new VoiceInterfaceCommand(m_manager, 
			name, iconSrc, description,
			tem->id(), ui.leVisibleTrigger->text(), ui.cbShowIcon->isChecked());
	voiceInterfaceCommand->assignAction(m_manager, tem->receiver(), tem->slot());
	return voiceInterfaceCommand;
}


VoiceInterfaceCommandTemplate* CreateVoiceInterfaceCommandWidgetPrivate::getCurrentTemplate()
{
	QList<VoiceInterfaceCommandTemplate*> voiceInterfaceCommandTemplates = m_manager->getVoiceInterfaceCommandTemplates();
	int currentIndex = ui.cbAction->currentIndex();
	if ((currentIndex == -1) || (currentIndex > voiceInterfaceCommandTemplates.count())) {
		KMessageBox::information(this, i18n("Please select a valid action"));
		return false;
	}

	return voiceInterfaceCommandTemplates[currentIndex];
}

void CreateVoiceInterfaceCommandWidgetPrivate::initFromTemplate()
{
	VoiceInterfaceCommandTemplate* tem = getCurrentTemplate();
	if (!tem) return;

	VoiceInterfaceCommand *c = new VoiceInterfaceCommand(m_manager, tem);
	emit commandSuggested(c); //command will be deleted by receiver
}

bool CreateVoiceInterfaceCommandWidgetPrivate::init(Command* command)
{
	VoiceInterfaceCommand *c = dynamic_cast<VoiceInterfaceCommand*>(command);
	if (!c) return false;

	int selectedActionIndex = ui.cbAction->findData(c->id());
	kDebug() << c->id() << selectedActionIndex;
	ui.cbAction->setCurrentIndex(selectedActionIndex);

	ui.leVisibleTrigger->setText(c->visibleTrigger());
	ui.cbShowIcon->setChecked(c->showIcon());
	return true;
}

bool CreateVoiceInterfaceCommandWidgetPrivate::isComplete()
{
	return (ui.cbAction->currentIndex() != -1);
}

