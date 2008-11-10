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
#include <simonsound/recwidget.h>
#include <speechmodelmanagement/trainingmanager.h>

#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KMessageBox>


TrainSamplePage::TrainSamplePage(const QString& prompt, int nowPage, int maxPage, const QString name, QWidget *parent) : QWizardPage(parent)
{
	this->prompt = prompt;
	QString title = i18n("Seite %1 von %2", nowPage, maxPage);
	setTitle(name+": "+title);


	fileName = QString(prompt).replace(" ", "_")+"_S"+QString::number(nowPage)+"_"
				+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");

	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *desc = new QLabel(i18n("Bitte nehmen Sie den unten angeführten Text jetzt auf.\n\nAchten Sie auf eine natürliche Aussprache!\n\nTipp: Lassen Sie am besten vor und nach dem Gesprochenen ca. 1 bis 2 Sekunden Pause für beste Ergebnisse!\n"), this);
	desc->setWordWrap(true);
	recorder = new RecWidget("", prompt, 
				  TrainingManager::getInstance()->getTrainingDir()+
					QDir::separator()+fileName+".wav", this);
	lay->addWidget(desc);
	lay->addWidget(recorder);

	
	connect(recorder, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(recorder, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
}

bool TrainSamplePage::submit()
{
	bool succ = TrainingManager::getInstance()->addSample(fileName, prompt.toUpper());
	if (!succ)
	{
		KMessageBox::error(this, i18n("Konnte Trainingssample nicht zum Datenschatz hinzufügen.\n\nDies deutet auf interne Datenkorruption hin."));
		cleanUp();
	}
	
	return succ;
}

bool TrainSamplePage::cleanUp()
{
	bool succ=true;
	if (recorder->hasRecordingReady()) 
	{
		succ = recorder->deleteSample();
		if (!succ)
			KMessageBox::error(this, i18n("Konnte Trainingssample nicht löschen."));
	}
	
	return succ;
}

bool TrainSamplePage::isComplete() const
{
	Q_ASSERT(recorder);

	return recorder->hasRecordingReady();
}

TrainSamplePage::~TrainSamplePage()
{
	recorder->deleteLater();
}
