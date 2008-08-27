#include "commandmanager.h"

bool CommandManager::trigger(const QString& triggerName)
{
	if (!commands) return false;

	bool done=false;
	for (int i=0; i < commands->count(); i++)
	{
		if (!commands->at(i)) continue;

		if (commands->at(i)->getTrigger() == triggerName)
		{
			if (commands->at(i)->trigger())
				done=true;

			//uncomment the following line if you don't want to allow multiple commands with the
			//same name. (this is just commented out for testing)
			//break;
		}
	}
	return done;
}

bool CommandManager::deleteCommand(Command *command)
{
	if (!commands) return false;

	if (commands->removeAll(command) > 0)
		return save();

	return false;
}
