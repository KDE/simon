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

/**
 * \brief Constructor - Inits the gui
 * \author Peter Grasch
 * @param parent The parent of the page
 */
ImportDictWiktionaryPage::ImportDictWiktionaryPage(QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbDesc = new QLabel (this);
	lbDesc->setText(tr("Das Wiktionary ist ein freies Wörterbuch der Wikimedia foundation und veröffentlicht Dumps seiner Datenbank die frei heruntergeladen werden können."));
	lbDesc->setWordWrap(true);
	
	
	QHBoxLayout *fileLay = new QHBoxLayout();
	lbWikiPath = new QLabel(this);
	lbWikiPath->setText(tr("Wiktionary-Dump:"));
	
	QRadioButton *importLocal = new QRadioButton(tr("Lokale Datei"), this);
	importLocal->setChecked(true);
	
	pbWikiSelectFolder = new QPushButton(this);
	pbWikiSelectFolder->setIcon(QIcon(":/images/icons/document-open.svg"));
	leWikiPath = new QLineEdit(this);
	registerField("wikiFileName*", leWikiPath);
	fileLay->addWidget(lbWikiPath);
	fileLay->addWidget(leWikiPath);
	fileLay->addWidget(pbWikiSelectFolder);
	
	connect(pbWikiSelectFolder, SIGNAL(clicked()), this, SLOT(setFile()));
	
	QRadioButton *importRemote = new QRadioButton(tr("Direkt Herunterladen"), this);

	lay->addWidget(lbDesc);
	lay->addWidget(importLocal);
	lay->addLayout(fileLay);
	lay->addWidget(importRemote);
	this->remoteList = new QListWidget(this);
	remoteList->setMinimumHeight(100);
	remoteList->setMaximumHeight(153);
	lay->addWidget(remoteList);
	this->setLayout(lay);
	registerField("importLocal", importLocal, "checked", SIGNAL(toggled(bool)));
	registerField("importRemote", importRemote, "checked", SIGNAL(toggled(bool)));

	setTitle(tr("Importiere Wiktionary Wörterbuch"));
	
	connect(importLocal, SIGNAL(toggled(bool)), this, SLOT(resambleImportLocal(bool)));
	connect(importLocal, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(leWikiPath, SIGNAL(textChanged( const QString& )), this, SIGNAL(completeChanged()));
	connect(remoteList, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
}


/**
 * \brief Changes the gui to activate/deactivate the widgets to display what we are doing (local/remote-import)
 * \author Peter Grasch
 * @param isTrue if set to true we go for the "import-local-look"
 */
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


/**
 * \brief Imports the given html list of available wikis
 * \author Peter Grasch
 * @param list Path to the list
 */
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

/**
 * \brief Loads the list of available wikis from the internet
 * \author Peter Grasch
 * 
 * Loads the list from http://download.wikimedia.org/backup-index.html using
 * the QuickDownloader class
 */
void ImportDictWiktionaryPage::loadList()
{
	QuickDownloader *qDownloader = new QuickDownloader(this);
	connect(qDownloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));

	QString url = "http://download.wikimedia.org/backup-index.html";
	if (!qDownloader->download(url))
		QMessageBox::critical(this, tr("Fehler beim Herunterladen"), tr("Konnte ")+url+tr(" nicht herunterladen."));
	
}

/**
 * \brief Returns true if we have entered what we need for the given method (local/remote)-import
 * \author Peter Grasch
 * @return completed?
 */
bool ImportDictWiktionaryPage::isComplete() const
{
	if (field("importLocal").toBool())
	{
		return !field("wikiFileName").toString().isEmpty();
	} else return !remoteList->selectedItems().isEmpty();
}

/**
 * \brief Returns the path to the wiki if we selected local import - else the link to it
 * \author Peter Grasch
 * @return the path to the wiki
 */
QString ImportDictWiktionaryPage::getPath()
{
	if (field("importLocal").toBool())
	{
		return field("wikiFileName").toString();
	} else {
		return remoteList->selectedItems().at(0)->data(Qt::UserRole).toString();
	}
}

/**
 * \brief Set the filename to a filename the user can enter with a filedialog
 * \author Peter Grasch
 */
void ImportDictWiktionaryPage::setFile()
{
	QFileDialog *dlg = new QFileDialog(this, tr("Zu importierende Textdatei öffnen"), QDir::currentPath());
	QStringList filters;
	filters << tr("Textdateien (%1)").arg("*.xml");
	filters << tr("BZip2 Komprimierte Dateien (%1)").arg("*.xml.bz2");
	dlg->setFilters(filters);
	if(!dlg->exec()) return;
	
	
	setField("wikiFileName", dlg->selectedFiles().at(0));
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictWiktionaryPage::~ImportDictWiktionaryPage()
{
}


