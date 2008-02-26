//
// C++ Interface: importlocalwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTLOCALWIZARDPAGE_H
#define IMPORTLOCALWIZARDPAGE_H

#include <QWizardPage>
#include "ui_importtrainingtextlocalpage.h"

/**
 * \class ImportLocalWizardPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Asks the user for the file to import
 * 
 * Extends QWizardpage; Uses the QT-Mandatory-field mechanism
 */
class ImportLocalWizardPage : public QWizardPage {
	Q_OBJECT
	private:
		Ui::ImportTrainingTextLocalPage ui;
	public slots:
		void setLocalSourceFile();
	public:
		ImportLocalWizardPage(QWidget* parent);
		int nextId() const {
			return 4;
		}
};


#endif
