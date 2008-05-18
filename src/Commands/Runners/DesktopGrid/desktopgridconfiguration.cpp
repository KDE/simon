//
// C++ Implementation: desktopgridconfiguration
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "desktopgridconfiguration.h"
#include "../../../SimonLib/Settings/settings.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the systemwidget
 */
DesktopGridConfiguration::DesktopGridConfiguration(QWidget* parent): SystemWidget(tr("Desktop-Gitter"), QIcon(":/images/icons/table.svg"), tr("Konfigurieren Sie das Gitter das die Mausdrücke-Simmulation erleichtert"), parent)
{
	help = tr("simon kann, wenn Sie keinen composite fähigen Desktop benutzen, Transparenz simmulieren.\n\nBenutzen Sie einen composite fähigen Desktop wie z.B.: \nLinux: Beryl/Compiz/Kwin (>= KDE 4.0)\nMicrosoft Windows: Windows 2000/XP/Vista\n\nsollten Sie hier ja sagen.\n\nWenn das Gitter dann nicht transparent angezeigt werden sollte, sagen Sie hier bitte nein.");

	ui.setupUi(this);
	connect(ui.leTrigger, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.cbRealTransparency, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
}


/**
 * \brief Are we done configuring?
 * \author Peter Grasch
 * @return True, if the trigger is set
 */
bool DesktopGridConfiguration::isComplete()
{
	return !ui.leTrigger->text().isEmpty();
}

/**
 * \brief Applys the values
 * \author Peter Grasch
 * @return Always true
 */
bool DesktopGridConfiguration::apply()
{
	Settings::set("Desktopgrid/Trigger", ui.leTrigger->text());
	Settings::set("Desktopgrid/RealTransparency", ui.cbRealTransparency->isChecked());
	return true;
}

/**
 * \brief Calls init()
 * \author Peter Grasch
 * @return Returns init()
 */
bool DesktopGridConfiguration::reset()
{
	return init();
}

/**
 * \brief Inits the systemwidget
 * \author Peter Grasch
 * @return Always true
 */
bool DesktopGridConfiguration::init()
{
	ui.leTrigger->setText(Settings::getS("Desktopgrid/Trigger"));
	ui.cbRealTransparency->setChecked(Settings::getB("Desktopgrid/RealTransparency"));
	return true;
}
