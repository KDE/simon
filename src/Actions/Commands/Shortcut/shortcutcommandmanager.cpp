#include "shortcutcommandmanager.h"
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmlshortcutcommand.h"
#include "shortcutcommand.h"
#include <KLocalizedString>


ShortcutCommandManager::ShortcutCommandManager(QObject *parent) :CommandManager(parent)  
{
	this->xmlShortcutCommand = new XMLShortcutCommand();
}


bool ShortcutCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<const ShortcutCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString ShortcutCommandManager::name() const
{
	return ShortcutCommand::staticCategoryText();
}

bool ShortcutCommandManager::load()
{
	QString commandPath = Settings::getS("Commands/Shortcut/PathToConfig");
	Logger::log(i18n("[INF] Lade Tastenkürzel von %1").arg(commandPath));

	bool ok = false;
	this->commands = xmlShortcutCommand->load(ok, commandPath);
	return ok;
}

bool ShortcutCommandManager::save()
{
	QString commandPath = Settings::getS("Commands/Shortcut/PathToConfig");
	Logger::log(i18n("[INF] Speichere Shortcut-Kommandos nach %1").arg(commandPath));
	return xmlShortcutCommand->save(commands, commandPath);
}


ShortcutCommandManager::~ShortcutCommandManager ()
{
	if (xmlShortcutCommand) xmlShortcutCommand->deleteLater();
}
