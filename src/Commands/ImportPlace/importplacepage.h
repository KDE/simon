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

#include "configureplacepage.h"
#include <QWizardPage>
#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
#include <QProgressBar>
#include "../command.h"


/**
 * \class ImportPlacePage
 * \author Susanne Tschernegg
 * \date 03.09.2007
 * \version 0.1
 * \brief This page adds the new command to the commandlist in the commandsettings.
 */
class ImportPlacePage : public QWizardPage
{
    Q_OBJECT
    
signals:
   void commandCreated(Command*);
    
public:
        ImportPlacePage(QWidget* parent);

        ~ImportPlacePage();

        QString getPlacePath();
        bool isComplete() const;
        void createCommand(QString name, QString value);

private:
    QLabel *label;
    QProgressBar *progressbar;

};

#endif
