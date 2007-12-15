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
#include "iconbutton.h"

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
	
	
	QLabel *lIcon= new QLabel(this);
	lIcon->setText(tr("Icon: "));
	iconBtn = new IconButton(this);
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
	confLayout->addWidget(lIcon, 0, 0);
	confLayout->addWidget(iconBtn, 0, 1);
	confLayout->addWidget(lName, 1, 0);
	confLayout->addWidget(leName, 1, 1);
	confLayout->addWidget(lPath, 2, 0);
	confLayout->addWidget(leExec, 2, 1);
	confLayout->addWidget(lWorkingPath, 3, 0);
	confLayout->addWidget(leWorkingPath, 3, 1);
	
	vboxLayout->addWidget(lbDesc);
	vboxLayout->addLayout(confLayout);
	
	registerField("name*", leName);
	registerField("exec*", leExec);
	registerField("workdir", leWorkingPath);
}

const QString ConfigureProgramPage::getIcon()
{
	return iconBtn->getIconName();
}

void ConfigureProgramPage::init(QString iconsrc, QString name, QString exec, QString workingDirectory)
{
	lbDesc->setText(tr("Sie haben das Programm %1 gewählt. \n\nHier können Sie nun weitere Einstellungen vornehmen.\n").arg(name));
	iconBtn->setIconName(iconsrc);
	iconBtn->setIcon(QIcon(iconsrc));
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


