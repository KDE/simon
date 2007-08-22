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

#define AUDIO 1
#define OFFICE 2
#define DEVELOPMENT 3
#define GRAPHIC 4
#define INTERNET 5
#define OTHER 6
#define GAMES 7
#define SYSTEM 8
#define VIDEO 9

SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName("vboxLayout");
    
    lwCategories = new QListWidget(this);
    lwCategories->setObjectName("lwCategories");
    
    QListWidgetItem* itemAudio = new QListWidgetItem(lwCategories);
    itemAudio->setText(tr("Audio"));
    itemAudio->setData(Qt::UserRole, AUDIO);
     QListWidgetItem* itembueroprogramme = new QListWidgetItem(lwCategories);
    itembueroprogramme->setText(tr("Büroprogramme"));
    itembueroprogramme->setData(Qt::UserRole, OFFICE);
     QListWidgetItem* itemEntwicklung = new QListWidgetItem(lwCategories);
    itemEntwicklung->setText(tr("Entwicklung"));
    itemEntwicklung->setData(Qt::UserRole, DEVELOPMENT);
     QListWidgetItem* itemGrafik = new QListWidgetItem(lwCategories);
    itemGrafik->setText(tr("Grafik"));
    itemGrafik->setData(Qt::UserRole, GRAPHIC);
     QListWidgetItem* itemInternet = new QListWidgetItem(lwCategories);
    itemInternet->setText(tr("Internet"));
    itemInternet->setData(Qt::UserRole, INTERNET);
     QListWidgetItem* itemSonstiges = new QListWidgetItem(lwCategories);
    itemSonstiges->setText(tr("Sonstiges"));
    itemSonstiges->setData(Qt::UserRole, OTHER);
     QListWidgetItem* itemGames = new QListWidgetItem(lwCategories);
    itemGames->setText(tr("Spiele"));
    itemGames->setData(Qt::UserRole, GAMES);
     QListWidgetItem* itemSystem = new QListWidgetItem(lwCategories);
    itemSystem->setText(tr("System"));
    itemSystem->setData(Qt::UserRole, SYSTEM);
     QListWidgetItem* itemVideo = new QListWidgetItem(lwCategories);
    itemVideo->setText(tr("Video"));
    itemVideo->setData(Qt::UserRole, VIDEO);
    
    lwPrograms = new QListWidget(this);
    lwPrograms->setObjectName("lwPrograms");
    
    registerField("executable*", lwPrograms);

    vboxLayout->addWidget(lwCategories);
    vboxLayout->addWidget(lwPrograms);
    
    regMan = new RegistryManager();
    
    //connect(lwCategories, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(test()));
    connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
    //getAllFormats();
    
    
//init platform dependant backends
#ifdef __WIN32
    regMan = new RegistryManager();
#endif
    
    //connect(lwCategories, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(test()));
    connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
    //getAllFormats();
}

void SelectProgramPage::searchForPrograms()
{
#ifdef __WIN32
    //zuerst die endungen suchen und dann schauen, ob es ein openwithlist gibt ... dann noch mal oben schauen, welcher perceivedtype, etc. es ist
    //QMessageBox::information(this, "susitest", "bin im searchForPrograms");
    QListWidgetItem *item = lwCategories->currentItem();
    int categorie = item->data(Qt::UserRole).toInt();
    
    //get all formats from a special perceivedType
    QStringList *formatList = new QStringList();
    if(categorie==AUDIO)
        formatList = this->getAllFormats("audio");
    else if(categorie==OFFICE)
        formatList = this->getAllFormats("text");
    else if(categorie==DEVELOPMENT)
        formatList = this->getAllFormats("text");
    else if(categorie==GRAPHIC)
        formatList = this->getAllFormats("image");
    else if(categorie==INTERNET)
        formatList = this->getAllFormats("");
    else if(categorie==OTHER)
        formatList = this->getAllFormats("all");
    else if(categorie==GAMES)
        formatList = this->getAllFormats("");
    else if(categorie==SYSTEM)
        formatList = this->getAllFormats("");
    else if(categorie==VIDEO)
        formatList = this->getAllFormats("video");
    else
        return;
    
  //  QString fStr;
  //  for(int x=0; x<formatList->count(); x++)
  //      fStr += formatList->at(x);
  //  QMessageBox::information(this, "searchforprograms", fStr);
   // QString format = ".mp3";
    
    lwPrograms->clear();
    QStringList* progList = new QStringList();
    QStringList *progListForOneFormat = new QStringList();
    bool exists = false;
    for(int y=0; y<formatList->count(); y++)
    {
        //get all programs for one format
        progListForOneFormat = regMan->getAllPrograms(formatList->at(y));
        for(int z=0; z<progListForOneFormat->count(); z++)
        {
            if(!progListForOneFormat->at(z).contains(".exe", Qt::CaseInsensitive))
                continue;
            for(int a=0; a<progList->count(); a++)
            {
                if(QString::compare(progListForOneFormat->at(z), progList->at(a), Qt::CaseInsensitive)==0)
                {
                    exists = true;
                    continue;
                }
            }
            if(!exists)
            {
                progList->append(progListForOneFormat->at(z));
            }
            else exists = false;
        }
    }
    
    QListWidgetItem* lwItem;
    QString out;
    int startIndex;
    int length;
    for(int i=0; i<progList->count(); i++)
    {
        lwItem = new QListWidgetItem(lwPrograms);
        
        startIndex = progList->at(i).indexOf(".");
        length = progList->at(i).length();
        lwItem->setData(Qt::AccessibleTextRole, progList->at(i));
        out = progList->at(i);
        if(startIndex>=0)
        {
            out.remove(startIndex, length-startIndex);
        }
        lwItem->setText(out);
    }
#endif
}

QStringList* SelectProgramPage::getAllFormats(QString format)
{
   //QMessageBox::information(this, "getAllFormats", "bin drin");
#ifdef __WIN32
    QStringList* formatList = this->regMan->getAllFormats(format);
    QString allFormats;
    for(int i=0; i<formatList->count(); i++)
    {
       allFormats += formatList->at(i);
    }
#endif
    return formatList;
}

SelectProgramPage::~SelectProgramPage()
{}
    
QString SelectProgramPage::getExecPath()
{
    QString exeStr = lwPrograms->currentItem()->data(Qt::AccessibleTextRole).toString();
    //QString path = regMan->getPath(exeStr);   //gets the whole path
    return exeStr;
}

QString SelectProgramPage::getName()
{
    return lwPrograms->currentItem()->text();
}



