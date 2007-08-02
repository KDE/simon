//
// C++ Interface: importworkingwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTWORKINGWIZARDPAGE_H
#define IMPORTWORKINGWIZARDPAGE_H

#include <QWizardPage>
#include <QString>
#include <QWidget>
#include "quickdownloader.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "xmltrainingtext.h"
#include "importlocalwizardpage.h"
#include "xmltrainingtextlist.h"

/**
 * \class ImportWorkingWizardPage
 * \brief Imports the given trainingstext
 * \author Peter Grasch
 */
class ImportWorkingWizardPage : public QWizardPage {
	Q_OBJECT 
	public slots:
		void startImport(QString path);
		void processText(QString path);
		void parseFile(QString path);
	public:
		ImportWorkingWizardPage(QWidget* parent);
};

#endif
