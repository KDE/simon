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
#include <QProcess>
#include <KStandardDirs>
#include "../KDELib/desktopreader.h"

KDEProgramManager::KDEProgramManager()
{
	QStringList kdedirs;
	QStringList env = QProcess::systemEnvironment();
	
	int i=0;
	while (i < env.count())
	{
		if (env.at(i).startsWith("KDEDIRS"))
		{
			QString kenv = env.at(i);
			QString kdirs = kenv.remove("KDEDIRS=");
			kdedirs = kdirs.split(":");
			break;

		}
		i++; 
	}
	this->kdedirs = kdedirs;
}

bool KDEProgramManager::loadPrograms()
{
	this->programs->clear();

	QStringList pathToDesktopFiles = getKDEDirs();

	for (int i=0; i < pathToDesktopFiles.count(); i++)
	{
		pathToDesktopFiles.replace(i, pathToDesktopFiles.at(i)+"/share/applications/");
		
		if (!QDir(pathToDesktopFiles.at(i)).exists())
		{
			pathToDesktopFiles.removeAt(i);
			i--;
		}
	}



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
	DesktopReader deskReader;
	for (int i=0; i < desktopFiles.count(); i++)
	{
		Program *prog = deskReader.readDesktopFile(desktopFiles.at(i));
		
		if (prog) {
			QStringList strCats = kdeCategoriesToSimonCategories(deskReader.getStrCategories());

			ProgramCategoryList categories;
			
			for (int j=0; j < strCats.count(); j++)
			{
				ProgramCategory *category = getCategory(strCats.at(j));
				if (category) 
					categories.append(*category);
			}
			QString iconsrc = resolveIcon(deskReader.getIconname());
			if (!iconsrc.isEmpty())
				prog->setIcon(QIcon(iconsrc), iconsrc);
			prog->setCategories(categories);
			
			this->programs->append(*prog);
		}
	}
	return true;
}

QStringList KDEProgramManager::getKDEDirs()
{
	return this->kdedirs;
}

QString KDEProgramManager::resolveIcon(QString iconname)
{
	QStringList kdedirs = getKDEDirs();
	if (kdedirs.count()==0)
		return "";
	
	for (int i=0; i < kdedirs.count(); i++)
	{
		kdedirs.replace(i, kdedirs.at(i)+"/share/icons/");
	}
	QStringList tosearch;
	while (!kdedirs.isEmpty())
	{
		QStringList dirList = QDir(kdedirs.at(0)).entryList();
		
		for (int j= 0; j < dirList.count(); j++) 
			tosearch.append(kdedirs.at(0)+dirList.at(j)+"/48x48/apps/"+iconname+".png");
		kdedirs.removeAt(0);
	}

	QString iconsrc;
	for (int i=0; i < tosearch.count(); i++)
	{
		if (QFile::exists(tosearch.at(i)))
		{
			iconsrc=tosearch.at(i);
			break;
		}
	}
	return iconsrc;
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


