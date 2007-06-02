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
	this->addPage(createLocalImportPage());
	this->addPage(createRemoteImportPage());
	this->addPage(createFinishedPage());
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

QWizardPage* ImportTrainingTexts::createRemoteImportPage()
{
	ImportRemoteWizardPage *remoteImport = new ImportRemoteWizardPage(this);
	remoteImport->setTitle("Importieren aus dem Internet");
	QLabel *label = new QLabel(remoteImport);
	label->setWordWrap(true);
	label->setText("Hier können Sie Texte life aus dem Internet importieren.\n\nBitte haben Sie einen Moment Geduld während die Liste der verfügbaren Texte geladen wird.\n\nSobald die Liste vollständig geladen wurde, wählen Sie bitte einen Text aus der Liste und bestätigen Sie mit weiter.");
	
	QListWidget *textList = new QListWidget(remoteImport);
	textList->addItem("Test");
	
	QVBoxLayout *layout = new QVBoxLayout(remoteImport);
	layout->addWidget(label);
	layout->addWidget(textList);
	remoteImport->setLayout(layout);
	
	
	return remoteImport;
}

QWizardPage* ImportTrainingTexts::createLocalImportPage()
{
	ImportLocalWizardPage *localImport = new ImportLocalWizardPage(this);
	localImport->setTitle("Importieren aus Textdatei");
	QLabel *label = new QLabel(localImport);
	label->setText("Hier können Sie Textdateien importieren. Sollte ihr Text in\neinem anderen Format (z.B.: einem Word Dokument)\nvorliegen, können Sie ihn in der spezifischen Applikation\nöffnen, als Text speichern und diese Datei hier angeben.\n\nBitte wählen Sie nun die Datei aus die Sie gerne\nimportieren möchten:");
	
	QVBoxLayout *layout = new QVBoxLayout(localImport);
	
	QHBoxLayout *fileLay = new QHBoxLayout();
	QLineEdit *lePath = new QLineEdit(localImport);
	lePath->setReadOnly(true);
	QPushButton *pbSelectPath = new QPushButton(localImport);
	pbSelectPath->setIcon(QIcon(":/images/icons/document-open.svg"));
	
	fileLay->addWidget(lePath);
	fileLay->addWidget(pbSelectPath);
	
	layout->addWidget(label);
	layout->addLayout(fileLay);
	localImport->setLayout(layout);
	connect(pbSelectPath, SIGNAL(clicked()), this, SLOT(setLocalSourceFile()));
	
	localImport->registerField("Filename*", lePath);
	
	return localImport;
}

void ImportTrainingTexts::setLocalSourceFile()
{
	
	QStringList files = QFileDialog::getOpenFileNames(this, "Zu importierende Textdateien öffnen", QDir::currentPath(), "Textdateien (*.txt)");
	if (files.count() == 0) return;
	
	QObjectList children = page(2)->children();
	
	for (int i=0; i < children.count(); i++)
	{
		if (dynamic_cast<QLineEdit*>(children.at(i)))
		{
			dynamic_cast<QLineEdit*>(children.at(i))->setText(files.at(0));
		}
	}
}

QWizardPage* ImportTrainingTexts::createSourcePage()
{
	SelectSourceWizardPage *source = new SelectSourceWizardPage(this);
	
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
	
	source->setLocal(local);
	source->setRemote(remote);
	
	return source;
}

QWizardPage* ImportTrainingTexts::createWorkingPage()
{
	
}

QWizardPage* ImportTrainingTexts::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle("Text hinzugefügt");
	QLabel *label = new QLabel(finished);
	label->setText("Hiermit haben Sie den neuen Text hinzugefügt.\n\nEr wird nun neben allen anderen Texten in Ihrem\nTrainingsdialog gelistet.\n\nVielen Dank und viel Spaß mit dem neuen Text!");
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}





ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{}



void ImportLocalWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


ImportRemoteWizardPage::ImportRemoteWizardPage(QWidget *parent) : QWizardPage(parent)
{}



void ImportRemoteWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


SelectSourceWizardPage::SelectSourceWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

int SelectSourceWizardPage::nextId() const
{
	if (this->local->isChecked())
		return 2;
	else return 3;
}