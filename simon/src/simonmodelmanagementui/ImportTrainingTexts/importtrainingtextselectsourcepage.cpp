/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


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
	setTitle(i18n("Choose your Source"));
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
		return 3;
	} else {
		return 2;
	}
}

ImportTrainingTextSelectSourcePage::~ImportTrainingTextSelectSourcePage()
{
}
