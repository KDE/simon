//
// C++ Implementation: importtrainingtexts
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingtexts.h"


/**
 * \brief Constructor
 * \author Peter Grasch
 */
ImportTrainingTexts::ImportTrainingTexts(QWidget *parent) : QWizard(parent)
{
	this->addPage(createIntroPage());
	this->addPage(createSourcePage());
	setWindowTitle("Trainingstext importieren");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/importtexts.png"));
	
}


/**
 * \brief Shows the wizard
 * \author Peter Grasch
 */
void ImportTrainingTexts::start()
{
	show();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportTrainingTexts::~ImportTrainingTexts()
{
}


QWizardPage* ImportTrainingTexts::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle("Importieren eines neuen Trainingstextes");
	QLabel *label = new QLabel(intro);
	label->setText("Mit Hilfe dieses Assistenten können Sie neue Trainingstexte\n aus dem Internet oder aus lokalen Dateien importieren.\n\nSo wird das Trainieren von simon nie langweilig!");
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

QWizardPage* ImportTrainingTexts::createSourcePage()
{
	QWizardPage *source = new QWizardPage(this);
	source->setTitle("Auswahl der Quelle");
	QLabel *label = new QLabel(source);
	label->setText("Sie können Texte aus lokalen Textdateien importieren\noder simon-kompatible Texte aus dem Internet laden.\n\nBitte wählen Sie die gewünschte Quelle aus:\n");
	
	QRadioButton *local = new QRadioButton(source);
	local->setText("Lokale Textdatei");
	local->setChecked(true);
	
	QLabel *localHelp = new QLabel(source);
	localHelp->setText("Wählen Sie diese Option wenn Sie einen Text, den\nSie gerne mit in Verbindung mit simon verwenden\nmöchten, gespeichert haben.\n");
	
	QRadioButton *remote = new QRadioButton(source);
	remote->setText("Aus dem Internet laden");
	
	QLabel *remoteHelp = new QLabel(source);
	remoteHelp->setText("Wenn Sie diese Option wählen wird zuerst eine\nListe der vorhandenen Texte aus dem Internet\ngeladen aus welcher Sie sich dann einen Text\naussuchen und importieren können.");
	
	QVBoxLayout *layout = new QVBoxLayout(source);
	layout->addWidget(label);
	layout->addWidget(local);
	layout->addWidget(localHelp);
	layout->addWidget(remote);
	layout->addWidget(remoteHelp);
	source->setLayout(layout);
	
	return source;
}

QWizardPage* ImportTrainingTexts::createWorkingPage()
{
	
}

QWizardPage* ImportTrainingTexts::createFinishedPage()
{
	
}
