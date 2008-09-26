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


#include "importtrainingtextremotepage.h"
#include "../../../SimonLib/Logging/logger.h"
#include "../../../SimonLib/QuickDownloader/quickdownloader.h"
#include "xmltrainingtextlist.h"
#include "coreconfiguration.h"

#include <KMessageBox>

/**
 * \brief Constructor
 * @param parent Sets the parent of the page to the given parent
 */
ImportTrainingTextRemotePage::ImportTrainingTextRemotePage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(i18n("Importieren aus dem Internet"));
	ui.setupUi(this);
	registerField("textDownloadURL*", ui.lwTexts, "currentUserData", SIGNAL(currentRowChanged(int)));
}

/**
 * \brief Fetches the list of trainingtexts using the QuickDownloader
 * \author Peter Grasch
 */
void ImportTrainingTextRemotePage::initializePage()
{
	downloader = new QuickDownloader(this);

	Logger::log(i18n("[INF] Abrufen der Liste von verfügbaren Trainingstexten"));
	
	connect (downloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));
	downloader->download(CoreConfiguration::textOnlineUpdate());
}

/**
 * \brief Imports the xml list of available trainingtexts from the given path
 * \author Peter Grasch
 * @param path the path to import from
 * \see fetchList()
 */
void ImportTrainingTextRemotePage::importList(QString path)
{
	XMLTrainingTextList *tlist = new XMLTrainingTextList(path);
	if (!tlist->load(path))
		KMessageBox::error(this, i18n("Konnte Liste der Texte nicht öffnen.\n\nMöglicherweise ist der URL falsch konfiguriert oder beim Download ist ein Fehler aufgetreten."));
	QHash<QString, QString> textlist = tlist->getTrainingTextList();
	ui.lwTexts->clear();
	for (int i=0; i < textlist.count(); i++)
	{
		QListWidgetItem *item;
		item = new QListWidgetItem(ui.lwTexts);
		item->setText(textlist.keys().at(i));
		item->setData(Qt::UserRole, textlist.values().at(i));
		ui.lwTexts->addItem(item);
	}
	delete tlist;
	downloader->deleteLater();
}

ImportTrainingTextRemotePage::~ImportTrainingTextRemotePage()
{
}
