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

#include "importdictintropage.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <KLocalizedString>


/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictIntroPage::ImportDictIntroPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Import Dictionary"));

	QLabel *lbIntro = new QLabel(this);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("This assistant will help you to import a new dictionary.\n\nA dictionary "
"contains information about the known words like how they are written and how "
"they are pronounced.\n\nThe dictionary is an essential core-component of your "
"language model. Please ensure that every dictionary that you import is of "
"high quality as it will massivly impact your recognition performance.\n\nWe "
"suggest that you use the Voxforge English Dictionary which is a HTK "
"compatible lexicon and of very high quality."));

	QLabel *lbTarget = new QLabel(this);
	lbTarget->setText(i18n("Target:"));

	QComboBox *cbTarget = new QComboBox(this);
	cbTarget->addItem(i18n("Shadow Dictionary"));
	cbTarget->addItem(i18n("Active Dictionary"));
	registerField("targetType", cbTarget, "currentIndex", SIGNAL(currentIndexChanged(int)));

	QHBoxLayout *targetLay = new QHBoxLayout();
	targetLay->addWidget(lbTarget);
	targetLay->addWidget(cbTarget);

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(lbIntro);
	lay->addLayout(targetLay);
	setLayout(lay);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictIntroPage::~ImportDictIntroPage()
{
}

