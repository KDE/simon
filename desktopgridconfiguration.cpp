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
#include "settings.h"
#include <QVariant>

DesktopGridConfiguration::DesktopGridConfiguration(QWidget* parent): SystemWidget(tr("Desktop-Gitter"), QIcon(":/images/icons/table.svg"), tr("Konfigurieren Sie das Gitter das die Mausdrücke-Simmulation erleichtert"), parent)
{
	help = "simon kann, wenn Sie keinen composite fähigen Desktop benutzen, Transparenz simmulieren.\n\nBenutzen Sie einen composite fähigen Desktop wie z.B.: \nLinux: Beryl/Compiz/Kwin (>= KDE 4.0)\nMicrosoft Windows: Windows 2000/XP/Vista\n\nsollten Sie hier ja sagen.\n\nWenn das Gitter dann nicht transparent angezeigt werden sollte, sagen Sie hier bitte nein.";

	ui.setupUi(this);
}



bool DesktopGridConfiguration::apply()
{
	Settings::set("Desktopgrid/Trigger", ui.leTrigger->text());
	Settings::set("Desktopgrid/RealTransparency", ui.cbRealTransparency->isChecked());
	return true;
}

bool DesktopGridConfiguration::reset()
{
	return init();
}

bool DesktopGridConfiguration::init()
{
	ui.leTrigger->setText(Settings::get("Desktopgrid/Trigger").toString());
	ui.cbRealTransparency->setChecked(Settings::get("Desktopgrid/RealTransparency").toBool());
	return true;
}


DesktopGridConfiguration::~DesktopGridConfiguration()
{}
