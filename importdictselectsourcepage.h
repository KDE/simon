//
// C++ Interface: importdictselectsourcepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTSELECTSOURCEPAGE_H
#define IMPORTDICTSELECTSOURCEPAGE_H

#define HADIFIXBOMP 1
#define WIKTIONARY 2

#include <QWizardPage>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportDictSelectSourcePage : public QWizardPage
{
public:
	int getType();
    ImportDictSelectSourcePage(QWidget* parent);

    ~ImportDictSelectSourcePage();

};

#endif
