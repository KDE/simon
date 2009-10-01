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


#ifndef SIMON_ADDWORDINTROPAGE_H_6B00540FA9AA4CAEA57D7485E150ABA9
#define SIMON_ADDWORDINTROPAGE_H_6B00540FA9AA4CAEA57D7485E150ABA9

#include <QWizardPage>
#include <QVariant>
#include "ui_addwordintropage.h"

class KLineEdit;
/**
 \class AddWordIntroPage
 \author Peter Grasch
 \date 01.08.2007
 \brief Asks for the name of the word
 
 The WizardPage of the AddWordView-Wizard that manages the wordname;
 Uses a mandatory-field mechanism to ensure that there is a name.
 Also it tells the user what this wizard is about (kind of an 
 instruction-page)
*/
class AddWordIntroPage : public QWizardPage
{
    Q_OBJECT
private:
	Ui::AddWordIntroPage ui;
    
public:
    AddWordIntroPage(QWidget* parent);
    ~AddWordIntroPage() {}

};

#endif
