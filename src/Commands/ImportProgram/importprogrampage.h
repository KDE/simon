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

class Command;
class XMLCommand;
class QVBoxLayout;
class QLabel;
class QProgressBar;

/**
 * \class ImportProgramPage
 * \author Peter Grasch
 * \date 16.08.2007
 * \version 0.1
 * \brief This page creates a new command and adds it to the commandlist.
 */
class ImportProgramPage : public QWizardPage
{
    Q_OBJECT
public:
        ImportProgramPage(QWidget* parent);

        ~ImportProgramPage();

        XMLCommand *commandLoader;
        void createCommand(QString icon, QString name, QString value, QString workingDir);

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
        QProgressBar *progressbar;

        virtual bool isComplete() const;

 signals:
   void commandCreated(Command *command);

};

#endif
