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


#ifndef SIMON_IMPORTTRAININGTEXTADDPAGE_H_AF24F65507794B159E6935DCF9D975EC
#define SIMON_IMPORTTRAININGTEXTADDPAGE_H_AF24F65507794B159E6935DCF9D975EC

#include <QWizardPage>
#include "ui_importtrainingtextaddpage.h"

/**
 * \class ImportTrainingTextAddPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Asks the user for the file to import
 * 
 * Extends QWizardpage; Uses the QT-Mandatory-field mechanism
 */
class ImportTrainingTextAddPage : public QWizardPage {
Q_OBJECT
	private:
		Ui::ImportTrainingTextAddPage ui;
	public:
		ImportTrainingTextAddPage(QWidget* parent);
		bool isComplete() const;
		void initializePage();
		int nextId() const {
			return 4; //working page
		}
};


#endif

