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
#include <QVariant>

class QLabel;
class IconButton;


/**
 * \class $CLASSNAME
 * \author 
 * \date 16.08.2007
 * \version 0.1
 * \brief At this page the user can define a name for the program.
 */
class ConfigureProgramPage : public QWizardPage
{
Q_OBJECT
private:
    QLabel *lbDesc;
	IconButton *iconBtn;
	
public:
	ConfigureProgramPage(QWidget* parent);
	
	~ConfigureProgramPage();

	void setName(QString name) { setField("name", name); }
	void setExec(QString exec) { setField("exec", exec); }
	void setWorkingDirectory(QString workingDir) { setField("workdir", workingDir); }
	

	const QString getIcon();
	const QString getName() { return field("name").toString(); }
	const QString getExec() { return field("exec").toString(); }
	const QString getWorkingDir() { return field("workdir").toString(); }

	void init(QString iconsrc, QString name, QString exec, QString workingDirectory);
};

#endif
