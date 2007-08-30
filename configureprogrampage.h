//
// C++ Interface: configureprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGUREPROGRAMPAGE_H
#define CONFIGUREPROGRAMPAGE_H

#include <QWizardPage>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>

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
class ConfigureProgramPage : public QWizardPage
{
    
public:
        ConfigureProgramPage(QWidget* parent);

        ~ConfigureProgramPage();

        QVBoxLayout *vboxLayout;
        QLabel *label;
        QHBoxLayout *hboxLayoutName;
        QLabel *lName;
        QLineEdit *leName;
        QHBoxLayout *hboxLayoutPath;
        QLabel *lPath;
        QLineEdit *lePath;
        QHBoxLayout *hboxLayoutWorkingPath;
        QLabel *lWorkingPath;
        QLineEdit *leWorkingPath;

        void setProgName(QString name){progName = name;}
        void setExecPath(QString path){execPath = path;}
        void writeInformation();
        
//private:
    QString progName;
    QString execPath;
};

#endif
