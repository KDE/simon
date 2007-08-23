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
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include "command.h"
#include "xmlcommand.h"

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
    Q_OBJECT
public:
        ImportProgramPage(QWidget* parent);

        ~ImportProgramPage();

        XMLCommand *commandLoader;
        void createCommand(QString name, QString value);

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
        QProgressBar *progressbar;

        virtual bool isComplete() const;

 signals:
   void commandCreated(Command *command);

};

#endif
