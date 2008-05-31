#include "executablecommandmanager.h"
#include <QDebug>
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmlexecutablecommand.h"
#include "executablecommand.h"


ExecutableCommandManager::ExecutableCommandManager(QObject *parent) :CommandManager(parent)  
{
	this->xmlExecutableCommand = new XMLExecutableCommand();
}

bool ExecutableCommandManager::addCommand(Command *command)
{
// 	qDebug() << "ExecutableCommandManager::addCommand(Command *command)=" << command;
	if (dynamic_cast<ExecutableCommand*>(command))
	{
// 		qDebug() << "-> Cast successfull";
		this->commands->append(command);
		return save();
	} else
// 		qDebug() << "-> Cast unsuccessfull :(";
	return false;
}

const QString ExecutableCommandManager::name() const
{
	return ExecutableCommand::staticCategoryText();
}

bool ExecutableCommandManager::load()
{
	QString commandPath = Settings::getS("Commands/Executable/PathToConfig");
	Logger::log(tr("[INF] Lade Ausführbare-Kommandos von %1").arg(commandPath));

	bool ok = false;
	this->commands = xmlExecutableCommand->load(ok, commandPath);
	return ok;
}

bool ExecutableCommandManager::save()
{
	QString commandPath = Settings::getS("Commands/Executable/PathToConfig");
	Logger::log(tr("[INF] Speichere Ausführbare-Kommandos nach %1").arg(commandPath));
	return xmlExecutableCommand->save(commands, commandPath);
}

ExecutableCommandManager::~ExecutableCommandManager()
{
	if (xmlExecutableCommand) xmlExecutableCommand->deleteLater();
}
