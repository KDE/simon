#include "textmacrocommandmanager.h"
#include <KLocalizedString>
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmltextmacrocommand.h"
#include "textmacrocommand.h"

TextMacroCommandManager::TextMacroCommandManager(QObject *parent) : CommandManager(parent)
{
	this->xmlTextMacroCommand = new XMLTextMacroCommand();
}

bool TextMacroCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<TextMacroCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString TextMacroCommandManager::name() const
{
	return TextMacroCommand::staticCategoryText();
}

bool TextMacroCommandManager::load()
{
	QString commandPath = Settings::getS("Commands/TextMacro/PathToConfig");
	Logger::log(i18n("[INF] Lade Text-Makro-Kommandos von %1", commandPath));

	bool ok = false;
	this->commands = xmlTextMacroCommand->load(ok, commandPath);
	return ok;
}

bool TextMacroCommandManager::save()
{
	QString commandPath = Settings::getS("Commands/TextMacro/PathToConfig");
	Logger::log(i18n("[INF] Speichere Text-Makro-Kommandos nach %1", commandPath));
	return xmlTextMacroCommand->save(commands, commandPath);
}


TextMacroCommandManager::~TextMacroCommandManager()
{
	if (xmlTextMacroCommand) xmlTextMacroCommand->deleteLater();
}
