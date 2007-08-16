//
// C++ Interface: importprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTPROGRAMPAGE_H
#define IMPORTPROGRAMPAGE_H

#include <QWizardPage>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
/**
 * \class $CLASSNAME
 * \author Peter Grasch
 * \date
 * \version 0.1
 * \brief
 */
class ImportProgramPage : public QWizardPage
{
public:
        ImportProgramPage(QWidget* parent);

        ~ImportProgramPage();

        bool isComplete() const;

};

#endif
