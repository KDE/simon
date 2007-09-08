//
// C++ Implementation: configureprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configureprogrampage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

/**
*   \brief constructor
*
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage::ConfigureProgramPage(QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout = new QVBoxLayout(this);
	vboxLayout->setObjectName("vboxLayout");
	
	lbDesc = new QLabel(this);
	lbDesc->setWordWrap(true);
	
	
	QLabel *lName = new QLabel(this);
	lName->setText(tr("Name: "));
	QLineEdit *leName = new QLineEdit(this);
	QLabel *lPath = new QLabel(this);
	lPath->setText(tr("Program: "));
	QLineEdit *leExec = new QLineEdit(this);
	QLabel *lWorkingPath = new QLabel(this);
	lWorkingPath->setText(tr("Arbeitspfad: "));
	QLineEdit *leWorkingPath = new QLineEdit(this);
	
	QGridLayout *confLayout = new QGridLayout();
	confLayout->addWidget(lName, 0, 0);
	confLayout->addWidget(leName, 0, 1);
	confLayout->addWidget(lPath, 1, 0);
	confLayout->addWidget(leExec, 1, 1);
	confLayout->addWidget(lWorkingPath, 2, 0);
	confLayout->addWidget(leWorkingPath, 2, 1);
	
	vboxLayout->addWidget(lbDesc);
	vboxLayout->addLayout(confLayout);
	
	registerField("name*", leName);
	registerField("exec*", leExec);
	registerField("workdir", leWorkingPath);
}

void ConfigureProgramPage::init(QString name, QString exec, QString workingDirectory)
{
	lbDesc->setText(tr("Sie haben das Programm %1 gewählt. \n\nHier können Sie nun weitere Einstellungen vornehmen.\n").arg(name));
	setField("name", name);
	setField("exec", exec);
	setField("workdir", workingDirectory);
}


/**
*   \brief destructor
*
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage::~ConfigureProgramPage()
{}


