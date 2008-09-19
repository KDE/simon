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
#ifndef IMPORTTRAININGTEXTLOCALPAGE_H
#define IMPORTTRAININGTEXTLOCALPAGE_H

#include <QWizardPage>
#include "ui_importtrainingtextlocalpage.h"

/**
 * \class ImportTrainingTextLocalPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Asks the user for the file to import
 * 
 * Extends QWizardpage; Uses the QT-Mandatory-field mechanism
 */
class ImportTrainingTextLocalPage : public QWizardPage {
Q_OBJECT
	private:
		Ui::ImportTrainingTextLocalPage ui;
	public:
		ImportTrainingTextLocalPage(QWidget* parent);
		int nextId() const {
			return 4;
		}
};


#endif
