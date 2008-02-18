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
#include "importremotewizardpage.h"
#include "logger.h"
#include "settings.h"
#include <QMessageBox>

/**
 * \brief Constructor
 * @param parent Sets the parent of the page to the given parent
 */
ImportRemoteWizardPage::ImportRemoteWizardPage(QWidget *parent) : QWizardPage(parent)
{

}

/**
 * \brief Wrapper for the registerField function
 * \author Peter Grasch
 * @param name name of the field
 * @param widget widget to represent it
 * @param property the property to watch
 * @param changedSignal the signal to watch
 */
void ImportRemoteWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}

/**
 * \brief Fetches the list of trainingtexts using the QuickDownloader
 * \author Peter Grasch
 */
void ImportRemoteWizardPage::initializePage()
{
	downloader = new QuickDownloader(this);

	Logger::log(tr("[INF] Abrufen der Liste von verfügbaren Trainingstexten"));
	
	connect (downloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));
	downloader->download(Settings::getS("PathToTextOnlineUpdate"));
}

/**
 * \brief Imports the xml list of available trainingtexts from the given path
 * \author Peter Grasch
 * @param path the path to import from
 * \see fetchList()
 */
void ImportRemoteWizardPage::importList(QString path)
{
	XMLTrainingTextList *tlist = new XMLTrainingTextList(path);
	if (!tlist->load(path))
		QMessageBox::critical(this, tr("Konnte Datei nicht öffnen"), tr("Konnte Liste der Texte nicht öffnen.\n\nMöglicherweise ist der URL falsch konfiguriert oder beim Download ist ein Fehler aufgetreten."));
	QHash<QString, QString> textlist = tlist->getTrainingTextList();
	list->clear();
	for (int i=0; i < textlist.count(); i++)
	{
		QListWidgetItem *item;
		item = new QListWidgetItem(list);
		item->setText(textlist.keys().at(i));
		item->setData(Qt::UserRole, textlist.values().at(i));
		list->addItem(item);
	}
	delete tlist;
	downloader->deleteLater();
}
