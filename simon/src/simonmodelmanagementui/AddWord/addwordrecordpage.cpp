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


#include "addwordrecordpage.h"
#include <QDate>
#include <QTime>
#include <QDir>
#include <KMessageBox>
#include <QVBoxLayout>
#include <simonsound/recwidget.h>
#include <KStandardDirs>
#include <speechmodelmanagement/trainingmanager.h>

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordRecordPage::AddWordRecordPage(QWidget *parent)
 : QWizardPage(parent)
{
	setTitle(i18n("Aufnehmen des Wortes"));
	ui.setupUi(this);

	rec1 = 0;
	rec2 = 0;
}

bool AddWordRecordPage::isComplete() const
{
	// 	return true;
	//make recordings mandatory
	return rec1 && rec2 && rec1->hasRecordingReady() && rec2->hasRecordingReady(); 
}

QString AddWordRecordPage::getSamplesDir()
{
	return TrainingManager::getInstance()->getTrainingDir();
// 	KConfig config( KStandardDirs::locate("config", "localsamplesrc") );
// 	KConfigGroup cGroup(&config, "");
// 	QString samplePath = cGroup.readEntry("SamplePath", KStandardDirs::locate("appdata", "samples/"));

// 	return samplePath;
}

/**
 * \brief Sets the recwidgets-titles to the examples of the previous page (pulled out of the fields wordExamples1,2)
 * \author Peter Grasch
 */
void AddWordRecordPage::initializePage()
{
	if (rec1)
	{
		layout()->removeWidget(rec1);
		rec1->deleteLater();
	}
	if (rec2)
	{
		layout()->removeWidget(rec2);
		rec2->deleteLater();
	}
	
	QString dateTime = QDate::currentDate().toString ( "yyyy-MM-dd" ) +"_"+QTime::currentTime().toString("hh-mm-ss");
	QString filename1=field("wordExample1").toString().replace(" ", "_")+ "_1_"+dateTime;
	QString filename2=field("wordExample2").toString().replace(" ", "_")+ "_2_"+dateTime;

	filename1 = QFile::encodeName(filename1);
	filename2 = QFile::encodeName(filename2);



	emit recordingNamesGenerated(filename1, filename2);

	QString sampleDir = getSamplesDir();
	if (sampleDir.isEmpty()) {
		KMessageBox::error(this, i18n("Konnte Pfad nicht erstellen.\n\nBitte überprüfen Sie Ihre Schreibrechte im konfigurierten Sample-Pfad"));
		return;
	}

	rec1 = new RecWidget(i18n("Aufnahme 1:"), field("wordExample1").toString(),
			     sampleDir+"/"+filename1+".wav", this);
	rec2 = new RecWidget(i18n("Aufnahme 2:"), field("wordExample2").toString(),
			      sampleDir+"/"+filename2+".wav", this);

	
	connect(rec1, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(rec2, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(rec1, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
	connect(rec2, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));

	layout()->addWidget(rec1);
	layout()->addWidget(rec2);
}


/**
 * \brief Destructor; Calls cleanUp()
 * \author Peter Grasch
 * \see cleanUp()
 */
AddWordRecordPage::~AddWordRecordPage()
{
	if (rec1) rec1->deleteLater();
	if (rec2) rec2->deleteLater();
	if (layout()) delete layout();
}


