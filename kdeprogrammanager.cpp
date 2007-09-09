//
// C++ Implementation: kdeprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kdeprogrammanager.h"
#include <QDir>
#include <QDebug>
#include "desktopreader.h"

KDEProgramManager::KDEProgramManager()
{
}

bool KDEProgramManager::loadPrograms()
{
	QStringList pathToDesktopFiles;
	pathToDesktopFiles << "/usr/kde/3.5/share/applications";
	pathToDesktopFiles << "/usr/share/applications/";

	QStringList desktopFiles = QStringList();
	
	for (int i=0; i < pathToDesktopFiles.count(); i++)
	{
		QDir *dirHandle = new QDir(pathToDesktopFiles.at(i));
		if ((!dirHandle) || (!dirHandle->isReadable())) return false;
	
		QStringList dirsToCheck;
		dirsToCheck<<pathToDesktopFiles.at(i);
	
		QStringList allowedFileTypes;
		allowedFileTypes << "*.desktop";
		QStringList dirs;
		QStringList files;
	
		while (!dirsToCheck.isEmpty())
		{
			dirHandle->setPath(dirsToCheck.takeAt(0));
			dirs = dirHandle->entryList(QDir::Dirs);
			for (int i=2; i < dirs.count(); i++)
				dirsToCheck.append(dirHandle->path()+"/"+dirs[i]);
	
			files = dirHandle->entryList(allowedFileTypes,  QDir::Files);
			for (int i=0; i < files.count(); i++)
				desktopFiles.append(dirHandle->path()+"/"+files[i]);
		}
	}
	DesktopReader *deskReader = new DesktopReader();
	for (int i=0; i < desktopFiles.count(); i++)
	{
		Program *prog = deskReader->readDesktopFile(desktopFiles.at(i));
		
		if (prog) {
			QStringList strCats = kdeCategoriesToSimonCategories(deskReader->getStrCategories());

			ProgramCategoryList categories;
			
			for (int j=0; j < strCats.count(); j++)
			{
				ProgramCategory *category = getCategory(strCats.at(j));
				if (category) 
					categories.append(*category);
			}
			
			prog->setCategories(categories);
			
			this->programs->append(*prog);
		}
	}
	return true;
}

QStringList KDEProgramManager::kdeCategoriesToSimonCategories(QStringList kdeCategories)
{
	QStringList strCats;
	for (int i=0; i < kdeCategories.count(); i++)
	{
		QString kdeCat = kdeCategories.at(i);
		if (kdeCat == "Network")
			strCats << "Internet";
		else if (kdeCat == "Development")
		{
			strCats << "Development";
			strCats << "IDE";
		} else if (kdeCat == "Graphics")
			strCats << "Graphic";
		else if (kdeCat.contains("X-KDE-settings") || kdeCat == "System")
			strCats << "System";
		else if (kdeCat == "Games")
			strCats << "Games";
		else if (kdeCat == "Office")
			strCats << "Office";
		else if ((kdeCat == "Utility") || (kdeCat == "TextEditor"))
			strCats << "Utility";
		else if ((kdeCat == "Video") || (kdeCat == "AudioVideo") || (kdeCat == "Audio"))
			strCats << "Multimedia";
	}
	strCats << "All";
	return strCats;
}

ProgramList* KDEProgramManager::getPrograms(ProgramCategory category)
{
	if (!this->programs) return NULL;

	ProgramList *progList = new ProgramList();
	
	int i=0;
	while (i < this->programs->count())
	{
		if (programs->at(i).getCategories().contains(category))
			progList->append(programs->at(i));
		i++;
	}

	return progList;
}

KDEProgramManager::~KDEProgramManager()
{
}


