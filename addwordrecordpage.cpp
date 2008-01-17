//
// C++ Implementation: addwordrecordpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addwordrecordpage.h"
#include "settings.h"
#include <QDate>
#include <QTime>

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordRecordPage::AddWordRecordPage(QWidget *parent)
 : QWizardPage(parent)
{
	setTitle(tr("Aufnehmen des Wortes"));
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *desc = new QLabel(this);
	desc->setText(tr("Bitte nehmen Sie nun das hinzuzufügende Wort zweimal auf.\n\nBitte achten Sie darauf, das Wort deutlich, aber natürlich\nauszusprechen und vermeiden Sie Hintergrundgeräusche.\n"));
	lay->addWidget(desc);

	QString example1=field("wordExample1").toString();
	QString example2=field("wordExample2").toString();

	QString dateTime = QDate::currentDate().toString ( "yyyy-MM-dd" ) +"_"+QTime::currentTime().toString("hh-mm-ss");

	QString filename1=example1.replace(" ", "_")+ "_"+dateTime;
	QString filename2=example2.replace(" ", "_")+ "_"+dateTime;

	emit recordingNamesGenerated(filename1, filename2);

	rec1 = new RecWidget(tr("1: %1").arg(example1), Settings::getS("TempDir")+"/"+filename1+".wav", this);
	rec2 = new RecWidget(tr("2: %1").arg(example2), Settings::getS("TempDir")+"/"+filename2+".wav", this);
	lay->addWidget(rec1);
	lay->addWidget(rec2);
	
	connect(rec1, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(rec2, SIGNAL(recordingFinished()), this, SIGNAL(completeChanged()));
	connect(rec1, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
	connect(rec2, SIGNAL(sampleDeleted()), this, SIGNAL(completeChanged()));
}

/**
 * \brief Sets the recwidgets-titles to the examples of the previous page (pulled out of the fields wordExamples1,2)
 * \author Peter Grasch
 */
void AddWordRecordPage::initializePage()
{
	rec1->setTitle(field("wordExample1").toString());
	rec2->setTitle(field("wordExample2").toString());
}

/**
 * \brief Cleans up and removes the samples
 * \author Peter Grasch
 */
void AddWordRecordPage::cleanUp()
{
	if (rec1->hasRecordingReady())
		rec1->deleteSample();
	if (rec2->hasRecordingReady())
		rec2->deleteSample();
}


/**
 * \brief Destructor; Calls cleanUp()
 * \author Peter Grasch
 * \see cleanUp()
 */
AddWordRecordPage::~AddWordRecordPage()
{
	cleanUp();
}


