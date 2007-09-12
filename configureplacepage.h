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
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class ConfigurePlacePage : public QWizardPage
{
    
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

private slots:
        void changeName();
        void changeValue();
};

#endif
