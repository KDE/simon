#include "desktopgridcommandmanager.h"
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "screengrid.h"

DesktopGridCommandManager::DesktopGridCommandManager(QObject *parent) : CommandManager(parent)
{
}

const QString DesktopGridCommandManager::name() const
{
	tr("Desktopgitter");
}

bool DesktopGridCommandManager::trigger(QString triggerName)
{
	if (triggerName != Settings::getS("Commands/DesktopGrid/Trigger")) return false;

	Logger::log(tr("[INF] Aktiviere Desktopgitter"));
	ScreenGrid *screenGrid = new ScreenGrid();
	screenGrid->show();
	return true;
}

bool DesktopGridCommandManager::load()
{
	return true;
}

bool DesktopGridCommandManager::save()
{
	return true;
}
