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

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class AddWordIntroPage : public QWizardPage
{
public:
    AddWordIntroPage(QWidget* parent);
	QString getName() { return field("name").toString(); }
    ~AddWordIntroPage();

};

#endif
