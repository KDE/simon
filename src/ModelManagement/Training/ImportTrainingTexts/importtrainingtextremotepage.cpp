//
// C++ Implementation: importremotewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingtextremotepage.h"
#include "../../../SimonLib/Logging/logger.h"
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/QuickDownloader/quickdownloader.h"
#include "xmltrainingtextlist.h"
#include <QMessageBox>

/**
 * \brief Constructor
 * @param parent Sets the parent of the page to the given parent
 */
ImportTrainingTextRemotePage::ImportTrainingTextRemotePage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(tr("Importieren aus dem Internet"));
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

	Logger::log(tr("[INF] Abrufen der Liste von verfügbaren Trainingstexten"));
	
	connect (downloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));
	downloader->download(Settings::getS("Internet/TextOnlineUpdate"));
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
		QMessageBox::critical(this, tr("Konnte Datei nicht öffnen"), tr("Konnte Liste der Texte nicht öffnen.\n\nMöglicherweise ist der URL falsch konfiguriert oder beim Download ist ein Fehler aufgetreten."));
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
