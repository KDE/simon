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

#include "createplacecommandwidget.h"
#include "placecommand.h"
#include "ImportPlace/importplacewizard.h"

CreatePlaceCommandWidget::CreatePlaceCommandWidget(QWidget* parent) : CreateCommandWidget(parent),
	importWizard(0)
{
	ui.setupUi(this);
	ui.urUrl->setMode(KFile::Directory | KFile::File | KFile::ExistingOnly);
	setWindowIcon(PlaceCommand::staticCategoryIcon());
	setWindowTitle(PlaceCommand::staticCategoryText());
	
	connect(ui.urUrl, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
	connect(ui.cbImportPlace, SIGNAL(clicked()), this, SLOT(showImportWizard()));
}

void CreatePlaceCommandWidget::showImportWizard()
{
	if (!importWizard) {
		importWizard = new ImportPlaceWizard(this);
		connect(importWizard, SIGNAL(commandCreated(Command*)), this, SIGNAL(commandSuggested(Command*)));
	}
	
	importWizard->restart();
	importWizard->show();
}



bool CreatePlaceCommandWidget::isComplete()
{
	return !(ui.urUrl->url().isEmpty());
}

bool CreatePlaceCommandWidget::init(Command* command)
{
	Q_ASSERT(command);
	
	PlaceCommand *placeCommand = dynamic_cast<PlaceCommand*>(command);
	if (!placeCommand) return false;
	
	ui.urUrl->setUrl(placeCommand->getURL());
	return true;
}

Command* CreatePlaceCommandWidget::createCommand(const QString& name, const QString& iconSrc)
{
	return new PlaceCommand(name, iconSrc, ui.urUrl->url());
}

CreatePlaceCommandWidget::~CreatePlaceCommandWidget()
{
	if (importWizard) importWizard->deleteLater();
}
