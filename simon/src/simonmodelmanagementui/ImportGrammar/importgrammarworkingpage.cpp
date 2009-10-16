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


#include "importgrammarworkingpage.h"
#include "importgrammar.h"
#include <QFile>
#include <kio/job.h>
#include <kio/jobuidelegate.h>
#include <KStandardDirs>

ImportGrammarWorkingPage::ImportGrammarWorkingPage(QWidget* parent): QWizardPage(parent),
	completed(false),
	grammarImporter(0)
{
	setTitle(i18n("Analysis running..."));
	ui.setupUi(this);
}

void ImportGrammarWorkingPage::printStatus(QString status)
{
	ui.lbCurrentAction->setText(status);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::displayFileProgress(int progress, int max)
{
	ui.pbProgress->setMaximum(max);
	ui.pbProgress->setValue(progress);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::displayWholeProgress(int progress, int max)
{
	ui.pbTotal->setMaximum(max);
	ui.pbTotal->setValue(progress);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::processCompletion()
{
	this->completed = true;
	emit completeChanged();
	if (grammarImporter) {
		grammarImporter->deleteLater();
		grammarImporter=0;
	}
}

void ImportGrammarWorkingPage::initializePage()
{
	completed=false;
	emit completeChanged();

	grammarImporter = new ImportGrammar(this);
	connect(grammarImporter, SIGNAL(status(QString)), this, SLOT(printStatus(QString)));
	connect(grammarImporter, SIGNAL(fileProgress(int, int)), this, SLOT(displayFileProgress(int, int)));
	connect(grammarImporter, SIGNAL(allProgress(int, int)), this, SLOT(displayWholeProgress(int, int)));
	connect(grammarImporter, SIGNAL(grammarCreated()), this, SIGNAL(grammarCreated()));
	connect(grammarImporter, SIGNAL(terminated()), this, SLOT(processCompletion()));


	QStringList files = field("files").toStringList();

	int index=0;
	QStringList tempFiles;
	foreach (const QString& file, files)
	{
		KUrl srcUrl(file);
		QString targetPath = KStandardDirs::locateLocal("tmp", "grammarImport/"+
					QString::number(index)+"_"+srcUrl.fileName());
		KIO::FileCopyJob *job = KIO::file_copy(srcUrl, targetPath, -1, KIO::Overwrite);
		if (!job->exec()) {
			job->ui()->showErrorMessage();
			continue;
		} else
			tempFiles << targetPath;
		index++;
		delete job;
	}
	
	grammarImporter->setFiles(tempFiles);
	grammarImporter->setEncoding(field("encoding").toString());
	grammarImporter->setIncludeUnknown(field("includeUnknown").toBool());
	grammarImporter->start();
}

void ImportGrammarWorkingPage::cancel()
{
	if (grammarImporter && grammarImporter->isRunning())
		grammarImporter->terminate();
}


ImportGrammarWorkingPage::~ImportGrammarWorkingPage()
{
	if (grammarImporter) grammarImporter->deleteLater();
}


