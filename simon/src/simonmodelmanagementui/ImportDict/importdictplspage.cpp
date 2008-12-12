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

#include "importdictplspage.h"


/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictPLSPage::ImportDictPLSPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);

	ui.urPath->setMode(KFile::File|KFile::ExistingOnly);

	registerField("plsFilename*", ui.urPath, "url", SIGNAL(textChanged (const QString &)));
	setTitle(i18n("Import PLS Dictionary"));
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictPLSPage::~ImportDictPLSPage()
{
}


