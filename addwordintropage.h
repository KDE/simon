//
// C++ Interface: addwordintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ADDWORDINTROPAGE_H
#define ADDWORDINTROPAGE_H

#include <QWizardPage>
#include <QVariant>

class QLineEdit;
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
public:
    AddWordIntroPage(QWidget* parent);
	QString getName() { return field("name").toString(); }
    ~AddWordIntroPage() {}

};

#endif
