//
// C++ Interface: selectsourcewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTSOURCEWIZARDPAGE_H
#define SELECTSOURCEWIZARDPAGE_H

#include <QWizardPage>
#include "ui_importtrainingtextselectsourcepage.h"
class QRadioButton;
/**
 * \class ImportTrainingTextSelectSourcePage
 * \author Peter Grasch
 * \version 0.1
 * \brief Selects the type of text to import: local or remote
 */
class ImportTrainingTextSelectSourcePage :  public QWizardPage {
	Q_OBJECT

	private:
		Ui::ImportTrainingTextSelectSourcePage ui;
	
	public:
		ImportTrainingTextSelectSourcePage(QWidget* parent);
        	~ImportTrainingTextSelectSourcePage();
		int nextId() const;
};

#endif
