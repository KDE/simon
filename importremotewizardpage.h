//
// C++ Interface: importremotewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTREMOTEWIZARDPAGE_H
#define IMPORTREMOTEWIZARDPAGE_H

#include <QWizardPage>
#include <QString>
#include <QWidget>
#include <QListWidget>
#include "quickdownloader.h"
#include "xmltrainingtextlist.h"
#include "ui_importtrainingtextremotepage.h"

class QuickDownloader;
/**
 * \class ImportRemoteWizardPage
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
 * \brief Displays the remote-importPage for the Trainingtexts
 */
class ImportRemoteWizardPage : public QWizardPage {
	Q_OBJECT
	private:
		QuickDownloader *downloader;
		Ui::ImportRemotePage ui;
	private slots:
		void importList(QString path);
		
	public:
		void initializePage();
		ImportRemoteWizardPage(QWidget* parent);
		int nextId() const {
			return 4;
		}
        ~ImportRemoteWizardPage();
};

#endif
