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
	
	QWizardPage *source = createSourcePage();
	QWizardPage *local = createLocalImportPage();
	QWizardPage *remote = createRemoteImportPage();
	QWizardPage *working = createWorkingPage();
	
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
	
	this->addPage(source);
	this->addPage(local);
	this->addPage(remote);
	this->addPage(working);
	
	this->addPage(createFinishedPage());
	setWindowTitle("Trainingstext importieren");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/importtexts.png"));
	prevId = 0;
	
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


/**
 * \brief Creates the intropage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle("Importieren eines neuen Trainingstextes");
	QLabel *label = new QLabel(intro);
	label->setText("Mit Hilfe dieses Assistenten k�nnen Sie neue Trainingstexte\n aus dem Internet oder aus lokalen Dateien importieren.\n\nSo wird das Trainieren von simon nie langweilig!");
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

/**
 * \brief Creates the remoteimport
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createRemoteImportPage()
{
	ImportRemoteWizardPage *remoteImport = new ImportRemoteWizardPage(this);
	remoteImport->setTitle("Importieren aus dem Internet");
	QLabel *label = new QLabel(remoteImport);
	label->setWordWrap(true);
	label->setText("Hier k�nnen Sie Texte life aus dem Internet importieren.\n\nBitte haben Sie einen Moment Geduld w�hrend die Liste der verf�gbaren Texte geladen wird.\n\nSobald die Liste vollst�ndig geladen wurde, w�hlen Sie bitte einen Text aus der Liste und best�tigen Sie mit weiter.");
	
	QListWidget *textList = new QListWidget(remoteImport);
	
	QVBoxLayout *layout = new QVBoxLayout(remoteImport);
	layout->addWidget(label);
	layout->addWidget(textList);
	remoteImport->setLayout(layout);
	remoteImport->setList(textList);
	
	
	return remoteImport;
}

/**
 * \brief Creates the localimportpage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createLocalImportPage()
{
	ImportLocalWizardPage *localImport = new ImportLocalWizardPage(this);
	localImport->setTitle("Importieren aus Textdatei");
	QLabel *label = new QLabel(localImport);
	label->setText("Hier k�nnen Sie Textdateien importieren. Sollte ihr Text in\neinem anderen Format (z.B.: einem Word Dokument)\nvorliegen, k�nnen Sie ihn in der spezifischen Applikation\n�ffnen, als Text speichern und diese Datei hier angeben.\n\nBitte w�hlen Sie nun die Datei aus die Sie gerne\nimportieren m�chten:");
	
	QVBoxLayout *layout = new QVBoxLayout(localImport);
	
	QGridLayout *glInput = new QGridLayout();
	
	QHBoxLayout *fileLay = new QHBoxLayout();
	QLabel *lbPath = new QLabel();
	lbPath->setText("Datei:");
	QLineEdit* lePath = new QLineEdit(localImport);
	lePath->setReadOnly(true);
	QPushButton *pbSelectPath = new QPushButton(localImport);
	pbSelectPath->setIcon(QIcon(":/images/icons/document-open.svg"));
	
	fileLay->addWidget(lePath);
	fileLay->addWidget(pbSelectPath);
	
	QLabel *lbName = new QLabel(localImport);
	lbName->setText("Name des Textes: ");
	QLineEdit *leName = new QLineEdit(localImport);
	
	glInput->addWidget(lbName, 0,0);
	glInput->addWidget(leName, 0,1);
	
	glInput->addWidget(lbPath, 1,0);
	glInput->addLayout(fileLay, 1,1);
	
	layout->addWidget(label);
	layout->addLayout(glInput);
	
	localImport->setLayout(layout);
	localImport->setPathEdit(lePath);
	connect(pbSelectPath, SIGNAL(clicked()), localImport, SLOT(setLocalSourceFile()));
	
	localImport->registerField("Filename*", lePath);
	localImport->registerField("Textname*", leName);
	
	return localImport;
}


/**
 * \brief Creates the sourcepage
 * 
 * Here you can select where you want to import from (internet/file)
 * 
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createSourcePage()
{
	SelectSourceWizardPage *source = new SelectSourceWizardPage(this);
	
	source->setTitle("Auswahl der Quelle");
	QLabel *label = new QLabel(source);
	label->setText("Sie k�nnen Texte aus lokalen Textdateien importieren\noder simon-kompatible Texte aus dem Internet laden.\n\nBitte w�hlen Sie die gew�nschte Quelle aus:\n");
	
	QRadioButton *local = new QRadioButton(source);
	local->setText("Lokale Textdatei");
	local->setChecked(true);
	
	QLabel *localHelp = new QLabel(source);
	localHelp->setText("W�hlen Sie diese Option wenn Sie einen Text, den\nSie gerne mit in Verbindung mit simon verwenden\nm�chten, gespeichert haben.\n");
	
	QRadioButton *remote = new QRadioButton(source);
	remote->setText("Aus dem Internet laden");
	
	QLabel *remoteHelp = new QLabel(source);
	remoteHelp->setText("Wenn Sie diese Option w�hlen wird zuerst eine\nListe der vorhandenen Texte aus dem Internet\ngeladen aus welcher Sie sich dann einen Text\naussuchen und importieren k�nnen.");
	
	QVBoxLayout *layout = new QVBoxLayout(source);
	layout->addWidget(label);
	layout->addWidget(local);
	layout->addWidget(localHelp);
	layout->addWidget(remote);
	layout->addWidget(remoteHelp);
	source->setLayout(layout);
	
	source->setLocal(local);
	source->setRemote(remote);
	
	return source;
}

/**
 * \brief Creates the working page
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createWorkingPage()
{
	ImportWorkingWizardPage *working= new ImportWorkingWizardPage(this);
	working->setTitle("Text wird hinzugef�gt");
	QLabel *label = new QLabel(working);
	label->setText("Der gew�hlte Text wird hinzugef�gt.\n\nBitte haben Sie etwas Geduld.\n");
	QProgressBar *progress = new QProgressBar(working);
	progress->setMaximum(0);
	progress->setValue(0);
	
	QVBoxLayout *layout = new QVBoxLayout(working);
	layout->addWidget(label);
	layout->addWidget(progress);
	working->setLayout(layout);
	
	return working;
}

/**
 * \brief Creates the finished-page
 * @return the QWizardPage
 */
QWizardPage* ImportTrainingTexts::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle("Text hinzugef�gt");
	QLabel *label = new QLabel(finished);
	label->setText("Hiermit haben Sie den neuen Text hinzugef�gt.\n\nEr wird nun neben allen anderen Texten in Ihrem\nTrainingsdialog gelistet.\n\nVielen Dank und viel Spa� mit dem neuen Text!");
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}


/**
 * \brief To react on changing the page
 * @param id The new id
 */
void ImportTrainingTexts::idChanged(int id)
{
	if (id < prevId) {
		prevId = id;
		return;
	}
	
	if (id==3) //remote
	{
		((ImportRemoteWizardPage*) page(id))->fetchList();
	} else if (id == 4) //working
	{
		if (!((ImportLocalWizardPage*)page(2))->getField("Filename")
				     .toString().isEmpty())
			((ImportWorkingWizardPage*)currentPage())->startImport(
		  	   ((ImportLocalWizardPage*)page(2))->getField("Filename").toString());
		else ((ImportWorkingWizardPage*)currentPage())->startImport(
		       ((ImportRemoteWizardPage*)page(3))->getCurrentData());
	}
	prevId = id;
}

