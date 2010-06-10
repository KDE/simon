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
#include "trainingswizard.h"
#include "trainsamplepage.h"
#include "deviceinformationpage.h"
#include "sampledataprovider.h"
#include "sendsamplespage.h"
#include "trainsampleintropage.h"
#include "sscconfig.h"

#include <simonsound/soundserver.h>
#include <simonsound/trainsamplevolumepage.h>

#include <QWizardPage>
#include <QStringList>

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QFile>
#include <QDir>

#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardDirs>
#include <KDebug>

#include <math.h>

TrainingsWizard::TrainingsWizard(QWidget *parent) : SimonWizard(parent)
{
	setBanner("training");
	addPage(createIntroPage());

	m_infoPage = createDeviceDescPage();
	addPage(m_infoPage);

	if (SoundServer::getCalibrateVolume())
		addPage(new TrainSampleVolumePage());

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DeviceInformationPage* TrainingsWizard::createDeviceDescPage()
{
	return new DeviceInformationPage(this);
}

bool TrainingsWizard::init(qint32 userId, TrainingsType type, const QStringList& prompts, const QString& name)
{
	setWindowTitle(name);
	
	if (prompts.isEmpty()) return false;

	SampleDataProvider *sampleDataProvider = new SampleDataProvider(userId, type);
	int nowPage=1;
	int maxPage=prompts.count();
	sampleDataProvider->registerMicrophoneInfo(m_infoPage);

	foreach (QString prompt, prompts)
	{
		TrainSamplePage *page = new TrainSamplePage(prompt, nowPage++, maxPage, name, this);
		connect(this, SIGNAL(rejected()), page, SLOT(cleanUp()));
		addPage(page);

		sampleDataProvider->registerDataProvider(page);
	}

	addPage(new SendSamplePage(sampleDataProvider, this));
	addPage(createFinishedPage());
	return true;
}

QStringList TrainingsWizard::repeatPrompts()
{
	return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/repeat/repeat.prompts"));
}


QStringList TrainingsWizard::trainingPrompts()
{
	return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/training/training.prompts"));
}


QStringList TrainingsWizard::interviewQuestions()
{
	return parsePromptsFromFile(KStandardDirs::locate("appdata", "texts/interview/interview.prompts"));
}

QStringList TrainingsWizard::parsePromptsFromFile(const QString& path)
{
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) return QStringList();

	QStringList prompts;

	while (!f.atEnd())
		prompts << QString::fromUtf8(f.readLine()).trimmed();

	return prompts;
}

int TrainingsWizard::collectSamples(TrainingsType type, qint32 userId)
{
	if (!cleanUp())
		return -1;
	QString name;
	QStringList prompts;
	switch (type) {
		case Repeating: {
			name = i18n("Repeating");
			prompts = repeatPrompts();
			break;
				}
		case Training: {
			name = i18n("Training");
			prompts = trainingPrompts();
			break;
				}
		case Interview: {
			name = i18n("Interview");
			prompts = interviewQuestions();
			break;
				}
	}
	if (!init(userId, type, prompts, name)) {
		KMessageBox::sorry(this, i18n("Invalid set of prompts"));
		return -1;
	}

	int ret = exec();

	return ret;
}


QWizardPage* TrainingsWizard::createIntroPage()
{
	return new TrainSampleIntroPage(this);
}

QWizardPage* TrainingsWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Training completed"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(i18n("The samples have been collected. They will now be transferred to the server."));
	lbFinished->setWordWrap(true);
	QVBoxLayout *lay = new QVBoxLayout(finished);
	lay->addWidget(lbFinished);
	finished->setLayout(lay);
	return finished;
}

void TrainingsWizard::submit()
{
	int i=0;
	forever
	{
		QWizardPage *plainPage = page(i++);
		if (!plainPage) break;
		
		TrainSamplePage *page = dynamic_cast<TrainSamplePage*>(plainPage);
		if (!page) 
			continue;
		
		page->submit();
	}
}

bool TrainingsWizard::cleanUp()
{
	//clean up the samples
	QDir d(SSCConfig::sampleDirectory());
	
	QStringList files = d.entryList(QDir::NoDotAndDotDot);
	bool all = true;
	QString sampleDirectory = SSCConfig::sampleDirectory();
	foreach (const QString file, files) {
		if (!QFile::remove(sampleDirectory+QDir::separator()+file)) {
			all = false;
			kDebug() << sampleDirectory+QDir::separator()+file;
		}
	}
	if (!all)
		KMessageBox::sorry(this, i18n("Could not clean sample directory %1.\n\nPlease remove all files manually", sampleDirectory));
	return all;
}

TrainingsWizard::~TrainingsWizard()
{

}
