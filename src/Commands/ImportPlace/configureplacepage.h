//
// C++ Interface: configureplacepage
//
// Description:
//
//
// Author:Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGUREPLACEPAGE_H
#define CONFIGUREPLACEPAGE_H

#include <QWizardPage>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>


/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date 12.09.2007
 * \version 0.1
 * \brief The priviously settings where listed here (name and value). The user can afterwards change this settings and also set a workingdirectory.
 */
class ConfigurePlacePage : public QWizardPage
{
Q_OBJECT
    
public:
        ConfigurePlacePage(QWidget* parent);

        ~ConfigurePlacePage();

        QVBoxLayout *vboxLayout;
        QLabel *label;
        QHBoxLayout *hboxLayoutName;
        QLabel *lName;
        QLineEdit *leName;
        QHBoxLayout *hboxLayoutPath;
        QLabel *lPath;
        QLineEdit *lePath;

        void setPlaceValue(QString value) {this->placeValue = value;}
        void setPlaceName(QString name) {this->placeName = name;}
        QString getPlaceValue() {return this->placeValue;}
        QString getPlaceName() {return this->placeName;}
        void setPlaceValue();
        void setPlaceName();
        
        void writeInformation();
        
private:
        QString placeValue;
        QString placeName;

//private slots:
//        void changeName();
//        void changeValue();
};

#endif
