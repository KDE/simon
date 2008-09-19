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
#ifndef IMPORTTRAININGTEXTWORKINGPAGE_H
#define IMPORTTRAININGTEXTWORKINGPAGE_H

#include <QWizardPage>
#include "ui_importtrainingtextworkingpage.h"
class QString;
class QWidget;

/**
 * \class ImportTrainingTextWorkingPage
 * \brief Imports the given trainingstext
 * \author Peter Grasch
 * \version 0.1
 */
class ImportTrainingTextWorkingPage : public QWizardPage {
	Q_OBJECT 
	private:
		Ui::ImportTextWorkingPage ui;
	public slots:
		void startImport(QString path);
		void processText(QString path);
		void parseFile(QString path);

		void initializePage();
	public:
		ImportTrainingTextWorkingPage(QWidget* parent);
};

#endif
