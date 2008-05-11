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
#include "simonlistwidget.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QIcon>
#include "settings.h"
#include "quickdownloader.h"

/**
 * \brief Constructor - Inits the gui
 * \author Peter Grasch
 * @param parent The parent of the page
 */
ImportDictWiktionaryPage::ImportDictWiktionaryPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	registerField("wikiFileName*", ui.leWikiPath, "currentUrl", SIGNAL(urlChanged(QString)));

	registerField("importWikiLocal", ui.rbImportLocal);
	registerField("importWikiRemote", ui.rbImportRemote);
	registerField("wikiRemoteURL", ui.lwRemoteList, "currentUserData",
			 SIGNAL(currentIndexChanged(int)));

	setTitle(tr("Importiere Wiktionary Wörterbuch"));
	
	connect(ui.rbImportLocal, SIGNAL(toggled(bool)), this, SLOT(resambleImportLocal(bool)));
	connect(ui.rbImportLocal, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(ui.leWikiPath, SIGNAL(urlChanged(QString)), this, SIGNAL(completeChanged()));
	connect(ui.lwRemoteList, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
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
		ui.leWikiPath->setEnabled(false);
		ui.leWikiPath->setEnabled(false);
		ui.lwRemoteList->setEnabled(true);
		
		loadList();
	} else
	{
		ui.leWikiPath->setEnabled(true);
		ui.leWikiPath->setEnabled(true);
		ui.lwRemoteList->setEnabled(false);
	}
}


/**
 * \brief Imports the given html list of available wikis
 * \author Peter Grasch
 * @param list Path to the list
 */
void ImportDictWiktionaryPage::importList(QString list)
{
	ui.lwRemoteList->clear();
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
			Settings::getS("Internet/WikiDumpPrefix")+wikiUrl+"/"+wikiName+"-"+date+Settings::getS("Internet/WikiDumpPostfix");
		
		txtList = txtList.mid(wikiStart+40);

		tmp = new QListWidgetItem(wikiName, ui.lwRemoteList);
		tmp->setData(Qt::UserRole, wikiUrl);
		ui.lwRemoteList->addItem(tmp);
		
		wikiStart = txtList.indexOf(pattern);
	}
	QuickDownloader *downloader = qobject_cast<QuickDownloader*>(sender());
	if (!downloader) return;
	downloader->deleteLater();
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

	QString url = Settings::getS("Internet/WikiDumpOverview");
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
	if (field("importWikiLocal").toBool())
	{
		return !field("wikiFileName").toString().isEmpty();
	} else return !ui.lwRemoteList->selectedItems().isEmpty();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictWiktionaryPage::~ImportDictWiktionaryPage()
{
}


