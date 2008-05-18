//
// C++ Implementation: selectsourcewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingtextselectsourcepage.h"

#include <QRadioButton>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent the parent of the qwizardpage
 */
ImportTrainingTextSelectSourcePage::ImportTrainingTextSelectSourcePage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(tr("Auswahl der Quelle"));
	registerField("importTrainingTextLocal", ui.rbLocal);
}

/**
 * \brief The next id of the wizard
 * \author Peter Grasch
 * If local: 2, else 3
 * @return 
 */
int ImportTrainingTextSelectSourcePage::nextId() const
{
	if (ui.rbLocal->isChecked())
	{
		return 2;
	} else {
		return 3;
	}
}

ImportTrainingTextSelectSourcePage::~ImportTrainingTextSelectSourcePage()
{
}
