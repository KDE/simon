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
#include <QSize>
#include <QDebug>
#include "program.h"
#include "kdeprogrammanager.h"
#include "programcategory.h"

/**
*   \brief Constructor
            Creates the wizardpage, where you can select a special categorie (audio, büroprogramme, etc.).
            To this categorie you get a list of programs, which provides standardformats of this categorie.
*
*   @author Susanne Tschernegg
*/
SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
#ifdef linux
	this->programManager = new KDEProgramManager();
#endif
#ifdef __Win32
	this->programManager = new WindowsProgramManager();
#endif
	ProgramCategoryList catList = programManager->readCategories();
	for (int i=0; i < catList.count(); i++)
	{
		QString text = catList.at(i).getDescription()+":\n";
		QStringList cats = catList.at(i).getFormats();
		for (int j=0; j<cats.count(); j++)
			text += "\n"+cats.at(j);

		QMessageBox::information(this, catList.at(i).getName(), text);
		
	}
	
	
        //init platform dependant backends
#ifdef __WIN32
        regMan = new RegistryManager();
#endif

        vboxLayout = new QVBoxLayout(this);
        vboxLayout->setObjectName("vboxLayout");

        lwCategories = new QListWidget(this);
        lwCategories->setObjectName("lwCategories");
        lwCategories->setIconSize(QSize(24,24));

        QListWidgetItem* itemAudio = new QListWidgetItem(lwCategories);
        itemAudio->setText(tr("Audio")); //audio-x-generic.svg
        itemAudio->setData(Qt::UserRole, AudioProgram);
        itemAudio->setIcon(QIcon(":/images/icons/audio-x-generic.svg"));

        QListWidgetItem* itembueroprogramme = new QListWidgetItem(lwCategories);
        itembueroprogramme->setText(tr("Büroprogramme")); //applications-office.svg
        itembueroprogramme->setData(Qt::UserRole, OfficeProgram);
        itembueroprogramme->setIcon(QIcon(":/images/icons/applications-office.svg"));

        QListWidgetItem* itemEntwicklung = new QListWidgetItem(lwCategories);
        itemEntwicklung->setText(tr("Entwicklung")); //applications-development.svg
        itemEntwicklung->setData(Qt::UserRole, DevelopmentProgram);
        itemEntwicklung->setIcon(QIcon(":/images/icons/applications-development.svg"));

        QListWidgetItem* itemGrafik = new QListWidgetItem(lwCategories);
        itemGrafik->setText(tr("Grafik")); //applications-graphics.svg
        itemGrafik->setData(Qt::UserRole, GraphicProgram);
        itemGrafik->setIcon(QIcon(":/images/icons/applications-graphics.svg"));

        QListWidgetItem* itemInternet = new QListWidgetItem(lwCategories);
        itemInternet->setText(tr("Internet")); //applications-internet.svg
        itemInternet->setData(Qt::UserRole, InternetProgram);
        itemInternet->setIcon(QIcon(":/images/icons/applications-internet.svg"));

        QListWidgetItem* itemSonstiges = new QListWidgetItem(lwCategories);
        itemSonstiges->setText(tr("Sonstiges")); // applications-other.svg
        itemSonstiges->setData(Qt::UserRole, OtherProgram);
        itemSonstiges->setIcon(QIcon(":/images/icons/applications-other.svg"));

        QListWidgetItem* itemGames = new QListWidgetItem(lwCategories);
        itemGames->setText(tr("Spiele")); //applications-games.svg
        itemGames->setData(Qt::UserRole, GamesProgram);
        itemGames->setIcon(QIcon(":/images/icons/applications-games.svg"));

        QListWidgetItem* itemSystem = new QListWidgetItem(lwCategories);
        itemSystem->setText(tr("System")); //applications-system.svg
        itemSystem->setData(Qt::UserRole, SystemProgram);
        itemSystem->setIcon(QIcon(":/images/icons/applications-system.svg"));

        QListWidgetItem* itemVideo = new QListWidgetItem(lwCategories);
        itemVideo->setText(tr("Video"));  //applications-multimedia.svg
        itemVideo->setData(Qt::UserRole, VideoProgram);
        itemVideo->setIcon(QIcon(":/images/icons/applications-multimedia.svg"));

        lwPrograms = new QListWidget(this);
        lwPrograms->setMaximumHeight(140);

        registerField("executable*", lwPrograms);



        vboxLayout->addWidget(lwCategories);
        vboxLayout->addWidget(lwPrograms);

        connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
        //connect(lwPrograms, sIGNAL(itemSelectionChanged()), this, SLOT(SelectProgramPage()));
        //getAllFormats();
}

/**
*   \brief searches for all programs, which contains the associated formats of a categorie
*
*   @author Susanne Tschernegg
*/
void SelectProgramPage::searchForPrograms()
{
#ifdef __WIN32
        //zuerst die endungen suchen und dann schauen, ob es ein openwithlist gibt ... dann noch mal oben schauen, welcher perceivedtype, etc. es ist
        QListWidgetItem *item = lwCategories->currentItem();
        int categorie = item->data(Qt::UserRole).toInt();

        //get all formats from a special perceivedType
        QStringList *formatList = new QStringList();
        if (categorie==AUDIO)
                formatList = this->getAllFormats("audio");
        else if (categorie==OFFICE)
                formatList = this->getAllFormats("text");
        else if (categorie==DEVELOPMENT)
                formatList = this->getAllFormats("text");
        else if (categorie==GRAPHIC)
                formatList = this->getAllFormats("image");
        else if (categorie==INTERNET)
                formatList = this->getAllFormats("");
        else if (categorie==OTHER)
                formatList = this->getAllFormats("all");
        else if (categorie==GAMES)
                formatList = this->getAllFormats("");
        else if (categorie==SYSTEM)
                formatList = this->getAllFormats("");
        else if (categorie==VIDEO)
                formatList = this->getAllFormats("video");
        else
                return;


        lwPrograms->clear();
        QStringList* progList = new QStringList();

        progList = regMan->getAllPrograms(formatList);


        QListWidgetItem* lwItem;
        QString out;
        int startIndex;
        int length;
        for (int i=0; i<progList->count(); i++) {
                lwItem = new QListWidgetItem(lwPrograms);

                startIndex = progList->at(i).lastIndexOf(".");
                length = progList->at(i).length();
                lwItem->setData(Qt::AccessibleTextRole, progList->at(i));
                out = progList->at(i);
                if (startIndex>=0) {
                        out.remove(startIndex, length-startIndex);
                }
                lwItem->setText(out);
        }
#endif
}

/**
*   \brief searches for all formats of a categorie
*
*   @autor Susanne Tschernegg
*/
QStringList* SelectProgramPage::getAllFormats(QString format)
{
        QStringList* formatList;
#ifdef __WIN32
        formatList = this->regMan->getAllFormats(format);
        QString allFormats;
        for (int i=0; i<formatList->count(); i++) {
                allFormats += formatList->at(i);
        }
#endif
        return formatList;
}

/**
*   \brief destructor
*
*   @autor Susanne Tschernegg
*/
SelectProgramPage::~SelectProgramPage()
{}

/**
*   \brief gets the whole exe-name of the program (e.g. program.exe)
*
*   @autor Susanne Tschernegg
*/
QString SelectProgramPage::getExecPath()
{
        QString exeStr = lwPrograms->currentItem()->data(Qt::AccessibleTextRole).toString();
        return exeStr;
}

/**
*   \brief gets the name of the program
*
*   @autor Susanne Tschernegg
*/
QString SelectProgramPage::getName()
{
        return lwPrograms->currentItem()->text();
}
