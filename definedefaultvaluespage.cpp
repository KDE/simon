//
// C++ Implementation: definedefaultvaluespage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "definedefaultvaluespage.h"
#include "settings.h"
#include <QCoreApplication>

DefineDefaultValuesPage::DefineDefaultValuesPage ( QWidget* parent ) : QWizardPage ( parent )
{
	setTitle("Setze Standardwerte...");
	this->complete=false;
	ui.setupUi(this);
}


DefineDefaultValuesPage::~DefineDefaultValuesPage()
{ }


bool DefineDefaultValuesPage::isComplete() const
{
	return this->complete;
}

/**
 * \brief Checks if the value is already set (it is not null) and sets it if it isn't
 *
 * @param option The setting to change
 * @param value The new value
 * \return true, if we changed anything
 */
bool DefineDefaultValuesPage::checkAndSet(QString option, QVariant value)
{
	if (Settings::get(option).isNull())
	{
		Settings::set(option, value);
		return true;
	}
	return false;
}

void DefineDefaultValuesPage::initializePage()
{
	this->complete=false;

	QString applicationPath = QCoreApplication::applicationDirPath()+"/";
	
	ui.pbProgress->setValue(0);
	
	checkAndSet("AskBeforeExit", true);
	ui.pbProgress->setValue(1);
	checkAndSet("AutoConnect", true);
	ui.pbProgress->setValue(2);
	checkAndSet("PathToCommands", applicationPath+"conf/commands.xml");
	ui.pbProgress->setValue(3);
	checkAndSet("PathToProgramCategories", applicationPath+"conf/categories.xml");
	ui.pbProgress->setValue(4);
	checkAndSet("PathToShortcuts", applicationPath+"conf/shortcuts.xml");
	ui.pbProgress->setValue(5);
	checkAndSet("PathToTexts", applicationPath+"texts/");
	ui.pbProgress->setValue(6);
	checkAndSet("SimonAutostart", true);
	ui.pbProgress->setValue(7);
	checkAndSet("StartJuliusdWhenRequired", false);
	ui.pbProgress->setValue(8);
	checkAndSet("TempDir", applicationPath+"tmp/");
	ui.pbProgress->setValue(9);
	checkAndSet("Desktopgrid/RealTransparency", false);
	ui.pbProgress->setValue(10);
	checkAndSet("Performance/MaxDisplayedWords", 500);
	ui.pbProgress->setValue(11);


	QWizardPage::initializePage();


	complete = true;
	emit done();
}

