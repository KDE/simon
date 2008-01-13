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

#include <QWizardPage>

/**
 * \class ImportDictSelectSourcePage
 * \brief Displays a WizardPage to let the user select the type of dict. to import
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
*/
class ImportDictSelectSourcePage : public QWizardPage
{
Q_OBJECT
public:
	int nextId() const;
    ImportDictSelectSourcePage(QWidget* parent);

    ~ImportDictSelectSourcePage();

};

#endif
