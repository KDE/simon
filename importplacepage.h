//
// C++ Interface: importplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTPLACEPAGE_H
#define IMPORTPLACEPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include "command.h"
#include "xmlcommand.h"

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class ImportPlacePage : public QWizardPage
{
    Q_OBJECT
public:
        ImportPlacePage(QWidget* parent);

        ~ImportPlacePage();

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
