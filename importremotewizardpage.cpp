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


/**
 * \brief Constructor
 * @param parent Sets the parent of the page to the given parent
 */
ImportRemoteWizardPage::ImportRemoteWizardPage(QWidget *parent) : QWizardPage(parent)
{}

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
void ImportRemoteWizardPage::fetchList()
{
	QuickDownloader *downloader = new QuickDownloader(this);

	Logger::log(tr("[INF] Abrufen der Liste von verfügbaren Trainingstexten"));
	
	connect (downloader, SIGNAL(downloadFinished(QString)), this, SLOT(importList(QString)));
	downloader->download("http://simon.pytalhost.org/texts/list.xml");
}

/**
 * \brief Imports the xml list of available trainingtexts from the given path
 * \author Peter Grasch
 * @param path the path to importfrom
 * \see fetchList()
 */
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
