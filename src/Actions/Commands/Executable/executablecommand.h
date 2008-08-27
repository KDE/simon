//
// C++ Interface: executablecommand
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 20078
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EXECUTABLECOMMAND_H
#define EXECUTABLECOMMAND_H

#include <KUrl>
#include "../command.h"

/**
 *	@class ExecutableCommand
 *	@brief Ressembles one exec-command
 *
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class ExecutableCommand : public Command{
Q_OBJECT
private:
	QString exe;
	KUrl workingDirectory;

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate();

public:
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;

	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ExecutableCommand(const QString& name, const QString& iconSrc, const QString& exe, const KUrl& workingDirectory) : Command(name, iconSrc)
    {
        this->exe = exe;
        this->workingDirectory = workingDirectory;
    }

    /**
    * @brief Returns the executable
    * 
    *	@author Peter Grasch
    */
    const QString getExecutable() const { return this->exe; }
    
    /**
    * @brief Returns the directory, the executable should be executed in (optional)
    *
    * @author Peter Grasch
    */
    const KUrl getWorkingDirectory() const {return this->workingDirectory;}
    
    

	void change(const QString& newName, const QString& newIconSrc, const QString& newExe, const QString& newWorkingDir)
	{ 
		this->exe = newExe;
		this->workingDirectory = newWorkingDir;
		Command::change(newName, newIconSrc);
	}

    ~ExecutableCommand() {}

};

#endif
