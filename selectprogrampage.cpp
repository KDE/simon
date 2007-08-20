//
// C++ Implementation: selectprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "selectprogrampage.h"
#include <QMessageBox>

/**
*   \brief Constructor
            Creates the wizardpage, where you can select a special categorie (audio, büroprogramme, etc.).
            To this categorie you get a list of programs, which provides standardformats of this categorie.
*
*   @autor Susanne Tschernegg
*/

SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName("vboxLayout");
    
    lwCategories = new QListWidget(this);
    lwCategories->setObjectName("lwCategories");
    
    QListWidgetItem* itemAudio = new QListWidgetItem(lwCategories);
    itemAudio->setText("Audio");
     QListWidgetItem* itembueroprogramme = new QListWidgetItem(lwCategories);
    itembueroprogramme->setText("Büroprogramme");
     QListWidgetItem* itemEntwicklung = new QListWidgetItem(lwCategories);
    itemEntwicklung->setText("Entwicklung");
     QListWidgetItem* itemGrafik = new QListWidgetItem(lwCategories);
    itemGrafik->setText("Grafik");
     QListWidgetItem* itemInternet = new QListWidgetItem(lwCategories);
    itemInternet->setText("Internet");
     QListWidgetItem* itemSpiele = new QListWidgetItem(lwCategories);
    itemSpiele->setText("Spiele");
     QListWidgetItem* itemSystem = new QListWidgetItem(lwCategories);
    itemSystem->setText("System");
     QListWidgetItem* itemVideo = new QListWidgetItem(lwCategories);
    itemVideo->setText("Video");
    
    lwPrograms = new QListWidget(this);
    lwPrograms->setObjectName("lwPrograms");

    vboxLayout->addWidget(lwCategories);
    vboxLayout->addWidget(lwPrograms);

    
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName("hboxLayout");
    
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    pbAbbrechen = new QPushButton(this);
    pbAbbrechen->setObjectName("pbAbbrechen");
    pbAbbrechen->setText("Abbrechen");
    
    pbHinzufuegen = new QPushButton(this);
    pbHinzufuegen->setObjectName("pbHinzufuegen");
    pbHinzufuegen->setText("Hinzufügen");

    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(pbAbbrechen);
    hboxLayout->addWidget(pbHinzufuegen);


    vboxLayout->addLayout(hboxLayout);
    
    regMan = new RegistryManager();
    
    //connect(lwCategories, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(test()));
    connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
    getAllFormats();
}

void SelectProgramPage::searchForPrograms()
{
    //zuerst die endungen suchen und dann schauen, ob es ein openwithlist gibt ... dann noch mal oben schauen, welcher perceivedtype, etc. es ist
    QMessageBox::information(this, "susitest", "bin im searchForPrograms");
    QListWidgetItem *item = lwCategories->currentItem();
    QString categorie = item->text();
    QString format = ".mp3";
    QStringList* progList = regMan->getAllPrograms(format);
    
    lwPrograms->clear();
    QListWidgetItem* lwItem;
    for(int i=0; i<progList->count(); i++)
    {
        lwItem = new QListWidgetItem(lwPrograms);
        lwItem->setText(progList->at(i));
    }
}

QStringList* SelectProgramPage::getAllFormats()
{
    QStringList* formatList = this->regMan->getAllFormats("image");
    QString allFormats;
    for(int i=0; i<formatList->count(); i++)
    {
       allFormats += formatList->at(i);
    }
}

SelectProgramPage::~SelectProgramPage()
{}


