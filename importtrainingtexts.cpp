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
	
	QVBoxLayout *layout = new QVBoxLayout(remoteImport);
	layout->addWidget(label);
	layout->addWidget(textList);
	remoteImport->setLayout(layout);
	remoteImport->setList(textList);
	
	
	return remoteImport;
}

QWizardPage* ImportTrainingTexts::createLocalImportPage()
{
	ImportLocalWizardPage *localImport = new ImportLocalWizardPage(this);
	localImport->setTitle("Importieren aus Textdatei");
	QLabel *label = new QLabel(localImport);
	label->setText("Hier können Sie Textdateien importieren. Sollte ihr Text in\neinem anderen Format (z.B.: einem Word Dokument)\nvorliegen, können Sie ihn in der spezifischen Applikation\nöffnen, als Text speichern und diese Datei hier angeben.\n\nBitte wählen Sie nun die Datei aus die Sie gerne\nimportieren möchten:");
	
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
	
// 	QHBoxLayout *nameLay = new QHBoxLayout();
	QLabel *lbName = new QLabel(localImport);
	lbName->setText("Name des Textes: ");
	QLineEdit *leName = new QLineEdit(localImport);
	
// 	nameLay->addWidget(lbName);
// 	nameLay->addWidget(leName);
	
// 	layout->addLayout(nameLay);
// 	layout->addLayout(fileLay);
	
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
	ImportWorkingWizardPage *working= new ImportWorkingWizardPage(this);
	working->setTitle("Text wird hinzugefügt");
	QLabel *label = new QLabel(working);
	label->setText("Der gewählte Text wird hinzugefügt.\n\nBitte haben Sie etwas Geduld.\n");
	QProgressBar *progress = new QProgressBar(working);
	progress->setMaximum(0);
	progress->setValue(0);
	
	QVBoxLayout *layout = new QVBoxLayout(working);
	layout->addWidget(label);
	layout->addWidget(progress);
	working->setLayout(layout);
	
	return working;
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




ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{}



void ImportLocalWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


void ImportLocalWizardPage::setLocalSourceFile()
{
	
	QStringList files = QFileDialog::getOpenFileNames(this, "Zu importierende Textdateien öffnen", QDir::currentPath(), "Textdateien (*.txt)");
	if (files.count() == 0) return;
	
	lePath->setText(files.at(0));
}


ImportRemoteWizardPage::ImportRemoteWizardPage(QWidget *parent) : QWizardPage(parent)
{}



void ImportRemoteWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}

void ImportRemoteWizardPage::fetchList()
{
	QuickDownloader *downloader = new QuickDownloader(this);
	
	connect (downloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));
	downloader->download("http://simon.pytalhost.org/texts/list.xml");
}

void ImportRemoteWizardPage::importList(QString path)
{
	XMLTrainingTextList *tlist = new XMLTrainingTextList(path);
	tlist->load();
	QHash<QString, QString> textlist = tlist->getTrainingTextList();
	
	QListWidgetItem *item;
	list->clear();
	for (int i=0; i < textlist.count(); i++)
	{
		item = new QListWidgetItem(list);
		item->setText(textlist.keys().at(i));
		item->setData(Qt::UserRole, textlist.values().at(i));
		list->addItem(item);
	}
}


SelectSourceWizardPage::SelectSourceWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

int SelectSourceWizardPage::nextId() const
{
	if (this->local->isChecked())
	{
		return 2;
	} else {
		return 3;
	}
}

ImportWorkingWizardPage::ImportWorkingWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

void ImportWorkingWizardPage::startImport(QString path)
{
	if (path.startsWith("http"))
	{
		QuickDownloader *qd = new QuickDownloader(this);
		connect(qd, SIGNAL(downloadFinished(QString)), this,
			SLOT(processText(QString)));
		qd->download(path);
	} else
		parseFile(path);
}

void ImportWorkingWizardPage::processText(QString path)
{
	QFileInfo fi = QFileInfo(path);
	QFile::copy(path, "texts/"+fi.fileName());
	QFile::remove(path);
	
	wizard()->next();
}

void ImportWorkingWizardPage::parseFile(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return;
	
	QStringList sents;
	QString tmp;
	QTextStream ts(&file);
	
	int sentend;
	QRegExp reg("(\\.|\\!|\\?)");
	QRegExp spec("(\\.\\.\\.|\\!\\!\\!|\\?\\?\\?)");
	QString currentLine;
	while ((!ts.atEnd()) || (!tmp.isEmpty()))
	{
		if (!ts.atEnd())
			currentLine = ts.readLine();
		else currentLine = "";
		
		if ((QString(tmp+currentLine).indexOf(spec) <= 
				   QString(tmp+currentLine).indexOf(reg)) && 
				   (QString(tmp+currentLine).indexOf(spec) != -1))
			sentend = QString(tmp+currentLine).indexOf(spec)+3;
		else if (QString(tmp+currentLine).indexOf(reg) != -1)
			 sentend = QString(tmp+currentLine).indexOf(reg)+1;
		else sentend = QString(tmp+currentLine).length();
		
		if (sentend == -1)
			tmp += currentLine;
		else  {
			QString sentence = QString(tmp+" "+currentLine).left(sentend).trimmed();
			if (!sentence.isEmpty()) sents << sentence;
			tmp = QString(tmp+currentLine).mid(sentend).trimmed();
		}
	}
	file.close();
	
	QFileInfo fi = QFileInfo(path);
	XMLTrainingText *text = new XMLTrainingText("texts/"+fi.fileName().left(fi.fileName().lastIndexOf("."))+".xml");
	text->setTitle(((ImportLocalWizardPage*)wizard()->page(2))->getField("Textname").toString());
	text->addPages(sents);
	text->save();
	
	wizard()->next();
}