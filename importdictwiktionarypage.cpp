//
// C++ Implementation: importdictwiktionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictwiktionarypage.h"
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QDebug>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QIcon>
#include "quickdownloader.h"

ImportDictWiktionaryPage::ImportDictWiktionaryPage(QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbDesc = new QLabel (this);
	lbDesc->setText(tr("Das Wiktionary ist ein freies Wörterbuch der Wikimedia foundation und veröffentlicht Dumps seiner Datenbank die frei heruntergeladen werden können."));
	lbDesc->setWordWrap(true);
	
	
	QHBoxLayout *fileLay = new QHBoxLayout();
	lbWikiPath = new QLabel(this);
	lbWikiPath->setText(tr("Wiktionary-Dump:"));
	
	QRadioButton *importLocal = new QRadioButton("Lokale Datei", this);
	importLocal->setChecked(true);
	
	pbWikiSelectFolder = new QPushButton(this);
	pbWikiSelectFolder->setIcon(QIcon(":/images/icons/document-open.svg"));
	leWikiPath = new QLineEdit(this);
	registerField("wikiFileName*", leWikiPath);
	fileLay->addWidget(lbWikiPath);
	fileLay->addWidget(leWikiPath);
	fileLay->addWidget(pbWikiSelectFolder);
	
	connect(pbWikiSelectFolder, SIGNAL(clicked()), this, SLOT(setFile()));
	
	QRadioButton *importRemote = new QRadioButton("Direkt Herunterladen", this);
	this->remoteList = new QListWidget();

	lay->addWidget(lbDesc);
	lay->addWidget(importLocal);
	lay->addLayout(fileLay);
	lay->addWidget(importRemote);
	lay->addWidget(this->remoteList);
	this->setLayout(lay);
	registerField("importLocal", importLocal, "checked", SIGNAL(toggled(bool)));
	registerField("importRemote", importRemote, "checked", SIGNAL(toggled(bool)));

	setTitle("Importiere Wiktionary Wörterbuch");
	
	connect(importLocal, SIGNAL(toggled(bool)), this, SLOT(resambleImportLocal(bool)));
	connect(importLocal, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(leWikiPath, SIGNAL(textChanged( const QString& )), this, SIGNAL(completeChanged()));
	connect(remoteList, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
}


void ImportDictWiktionaryPage::resambleImportLocal(bool isTrue)
{
	if (!isTrue)
	{
		lbWikiPath->setEnabled(false);
		leWikiPath->setEnabled(false);
		pbWikiSelectFolder->setEnabled(false);
		remoteList->setEnabled(true);
		
		loadList();
	} else
	{
		lbWikiPath->setEnabled(true);
		leWikiPath->setEnabled(true);
		pbWikiSelectFolder->setEnabled(true);
		remoteList->setEnabled(false);
	}
}


void ImportDictWiktionaryPage::importList(QString list)
{
	remoteList->clear();
	QFile listFile(list);
	if (!listFile.open(QIODevice::ReadOnly)) return;
	QTextStream *txtStream = new QTextStream(&listFile);
	QString txtList = txtStream->readAll();
	listFile.close();

	QRegExp pattern = QRegExp("href=\\\"..wiktionary/........\\\"\\>..wiktionary\\<\\/a\\>: \\<span class=\\'done");
	
	QString wikiUrl, wikiName;
	QListWidgetItem *tmp;
	QString date;
	int wikiStart = txtList.indexOf(pattern);
	while (wikiStart != -1)
	{
		wikiUrl = txtList.mid(wikiStart+6, 21);
		wikiName = txtList.mid(wikiStart+29, 12);
		date = wikiUrl.mid(wikiUrl.indexOf("/")+1);
		wikiUrl = 
			"http://download.wikimedia.org/"+wikiUrl+"/"+wikiName+"-"+date+"-pages-articles.xml.bz2";
		
		txtList = txtList.mid(wikiStart+40);

		tmp = new QListWidgetItem(wikiName, remoteList);
		tmp->setData(Qt::UserRole, wikiUrl);
		this->remoteList->addItem(tmp);
		
		wikiStart = txtList.indexOf(pattern);
	}
}

void ImportDictWiktionaryPage::loadList()
{
	QuickDownloader *qDownloader = new QuickDownloader(this);
	connect(qDownloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));

	QString url = "http://download.wikimedia.org/backup-index.html";
	if (!qDownloader->download(url))
		QMessageBox::critical(this, tr("Fehler beim Herunterladen"), tr("Konnte ")+url+tr(" nicht herunterladen."));
	
}

bool ImportDictWiktionaryPage::isComplete() const
{
	if (field("importLocal").toBool())
	{
		return !field("wikiFileName").toString().isEmpty();
	} else return !remoteList->selectedItems().isEmpty();
}

QString ImportDictWiktionaryPage::getPath()
{
	if (field("importLocal").toBool())
	{
		return field("wikiFileName").toString();
	} else {
		return remoteList->selectedItems().at(0)->data(Qt::UserRole).toString();
	}
}

void ImportDictWiktionaryPage::setFile()
{
	QFileDialog *dlg = new QFileDialog(this, "Zu importierende Textdatei öffnen", QDir::currentPath());
	QStringList filters;
	filters << "Textdateien (*.xml)";
	filters << "BZip2 Komprimierte Dateien (*.xml.bz2)";
	dlg->setFilters(filters);
	if(!dlg->exec()) return;
	
	
	setField("wikiFileName", dlg->selectedFiles().at(0));
}


ImportDictWiktionaryPage::~ImportDictWiktionaryPage()
{
}


