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
#include "trainsamplepage.h"
#include "sscconfig.h"
#include <simonsound/recwidget.h>

#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

#include <KLocalizedString>
#include <KMessageBox>


TrainSamplePage::TrainSamplePage(QString prompt_, int nowPage, int maxPage, const QString name, QWidget* parent) : QWizardPage(parent),
	prompt(prompt_),
	fileName( prompt_.replace(" ", "_").replace("/","_").remove("?").replace("\\", "_").remove("<").remove(">").remove("|").remove("\"")
		+ "_S"
		+ QString::number(nowPage)
		+ "_"
		+ QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") )
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QString title = i18n("Page %1 of %2", nowPage, maxPage);
	setTitle(name+": "+title);

	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *desc = new QLabel(i18n("Please record the Text below."
"\n\nTip: Leave about one to two seconds \"silence\" before and after you read "
"the text for best results!\n"), this);
	desc->setWordWrap(true);
	recorder = new RecWidget("", prompt, 
				  SSCConfig::sampleDirectory()+
					fileName, false, this);
	lay->addWidget(desc);
	lay->addWidget(recorder);

	connect(recorder, SIGNAL(recording()), this, SIGNAL(completeChanged()));
	connect(recorder, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(recorder, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
}


void TrainSamplePage::initializePage()
{
	if (field("powerRecording").toBool()) {
		if (!recorder->hasRecordingReady())
			recorder->record();
	}
}


bool TrainSamplePage::validatePage()
{
	recorder->stopRecording();
	recorder->stopPlayback();

	return true;
}

QStringList TrainSamplePage::getFileNames()
{
	QStringList fileNames = recorder->getFileNames();
	for (int i=0; i < fileNames.count(); i++)
	{
		QString fileName = fileNames[i];
		int last_sep = qMax(fileName.lastIndexOf("/"), fileName.lastIndexOf("\\"));

		fileName = fileName.mid(last_sep+1);
		fileName = fileName.left(fileName.lastIndexOf("."));
		fileNames.replace(i, fileName);
	}
		
	return  fileNames;
}

QStringList TrainSamplePage::getDevices()
{
	return recorder->getDevices();
}

bool TrainSamplePage::submit()
{
	return true;
//	bool succ = TrainingManager::getInstance()->addSample(fileName, prompt.toUpper());
//	if (!succ) {
//		KMessageBox::error(this, i18n("Couldn't send samples.\n\nThis indicates internal data corruption."));
//		cleanUp();
//	}
//	
//	return succ;
}

void TrainSamplePage::cleanupPage()
{
	recorder->stopRecording();
	recorder->stopPlayback();
}

bool TrainSamplePage::cleanUp()
{
	bool succ=true;
	if (recorder->hasRecordingReady()) 
	{
		succ = recorder->deleteAll();
		if (!succ)
			KMessageBox::error(this, i18n("Couldn't remove samples \"%1\".", getFileNames().join("\", \"")));
	}
	
	return succ;
}

bool TrainSamplePage::isComplete() const
{
	Q_ASSERT(recorder);

	//if (field("powerRecording").toBool())
		return true;
	//else
	//	return recorder->hasRecordingReady();
}

TrainSamplePage::~TrainSamplePage()
{
	recorder->deleteLater();
}
