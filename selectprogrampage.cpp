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

SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(Form);
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
    
    pbHinzufuegen = new QPushButton(this);
    pbHinzufuegen->setObjectName("pbHinzufuegen");

    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(pbAbbrechen);
    hboxLayout->addWidget(pbHinzufuegen);


    vboxLayout->addLayout(hboxLayout);
}


SelectProgramPage::~SelectProgramPage()
{}


