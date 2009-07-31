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
#include <QVariant>

#include <KLocalizedString>
#include <KMessageBox>


TrainSamplePage::TrainSamplePage(const QString& prompt, int nowPage, int maxPage, const QString name, QWidget *parent) : QWizardPage(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->prompt = prompt;
	QString title = i18n("Page %1 of %2", nowPage, maxPage);
	setTitle(name+": "+title);


	fileName = TrainingManager::getInstance()->htkify(prompt).replace(" ", "_")+"_S"+QString::number(nowPage)+"_"
				+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");

	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *desc = new QLabel(i18n("Please record the Text below."
"\n\nTip: Leave about one to two seconds \"silence\" before and after you read "
"the text for best results!\n"), this);
	desc->setWordWrap(true);
	recorder = new RecWidget("", prompt, 
				  TrainingManager::getInstance()->getTrainingDir()+
					fileName+".wav", this);
	lay->addWidget(desc);
	lay->addWidget(recorder);

	
	connect(recorder, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(recorder, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
}


void TrainSamplePage::initializePage()
{
	if (field("powerRecording").toBool()) {
		if (!recorder->hasRecordingReady())
			recorder->record();
	}

//	QTimer::singleShot(3000, recorder, SLOT(resizePromptLabel()));
}

bool TrainSamplePage::validatePage()
{
	recorder->stopRecording();
	recorder->stopPlayback();

	return true;
}

bool TrainSamplePage::submit()
{
	bool succ = TrainingManager::getInstance()->addSample(fileName, prompt.toUpper());
	if (!succ)
	{
		KMessageBox::error(this, i18n("Couldn't add samples to the corpus.\n\nThis indicates internal data corruption."));
		cleanUp();
	}
	
	return succ;
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
		succ = recorder->deleteSample();
		if (!succ)
			KMessageBox::error(this, i18n("Couldn't remove sample \"%1\".", getFileName()));
	}
	
	return succ;
}

bool TrainSamplePage::isComplete() const
{
	Q_ASSERT(recorder);

	if (field("powerRecording").toBool())
		return true;
	else
		return recorder->hasRecordingReady();
}

TrainSamplePage::~TrainSamplePage()
{
	recorder->deleteLater();
}
