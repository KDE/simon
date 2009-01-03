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


#include "importtrainingdirectoryintropage.h"
#include <QDir>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectoryIntroPage::ImportTrainingDirectoryIntroPage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	
	ui.urTrainingDataDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
	
	setTitle(i18n("Import Trainings Samples from Folder"));
	
	connect(ui.urTrainingDataDirectory, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
	registerField("directory*", ui.urTrainingDataDirectory, "url", SIGNAL(textChanged(QString)));
}


bool ImportTrainingDirectoryIntroPage::isComplete() const
{
	QString dir = ui.urTrainingDataDirectory->url().path();
	return ((!dir.isEmpty()) && QDir(dir).exists());
}


