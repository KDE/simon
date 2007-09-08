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

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
/**
 * \class $CLASSNAME
 * \author Peter Grasch
 * \date
 * \version 0.1
 * \brief
 */
class ConfigureProgramPage : public QWizardPage
{
private:
        QLabel *lbDesc;
	
public:
	ConfigureProgramPage(QWidget* parent);
	
	~ConfigureProgramPage();

	void setName(QString name) { setField("name", name); }
	void setExec(QString exec) { setField("exec", exec); }
	void setWorkingDirectory(QString workingDir) { setField("workdir", workingDir); }

	const QString getName() { return field("name").toString(); }
	const QString getExec() { return field("exec").toString(); }
	const QString getWorkingDir() { return field("workdir").toString(); }

	void init(QString name, QString exec, QString workingDirectory);
};

#endif
