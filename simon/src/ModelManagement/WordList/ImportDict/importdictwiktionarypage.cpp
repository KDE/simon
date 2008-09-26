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


#include "importdictwiktionarypage.h"
#include "simonlistwidget.h"
#include "../../../SimonLib/QuickDownloader/quickdownloader.h"
#include "coreconfiguration.h"
#include <QFile>
#include <QTextStream>
#include <KMessageBox>

/**
 * \brief Constructor - Inits the gui
 * \author Peter Grasch
 * @param parent The parent of the page
 */
ImportDictWiktionaryPage::ImportDictWiktionaryPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	registerField("wikiFileName*", ui.urWikiPath, "url", SIGNAL(textChanged(QString)));

	registerField("importWikiLocal", ui.rbImportLocal);
	registerField("importWikiRemote", ui.rbImportRemote);
	registerField("wikiRemoteURL", ui.lwRemoteList, "currentUserData",
			 SIGNAL(currentIndexChanged(int)));

	setTitle(i18n("Importiere Wiktionary Wörterbuch"));
	
	connect(ui.rbImportLocal, SIGNAL(toggled(bool)), this, SLOT(resambleImportLocal(bool)));
	connect(ui.rbImportLocal, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(ui.urWikiPath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
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
		ui.urWikiPath->setEnabled(false);
		ui.lwRemoteList->setEnabled(true);
		
		loadList();
	} else
	{
		ui.urWikiPath->setEnabled(true);
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
			CoreConfiguration::wikiDumpPrefix()+wikiUrl+"/"+wikiName+"-"+date+CoreConfiguration::wikiDumpPostfix();
		
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

	QString url = CoreConfiguration::wikiDumpOverview();
	if (!qDownloader->download(url))
		KMessageBox::error(this, i18n("Konnte %1 nicht herunterladen.", url));
	
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


