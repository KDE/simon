#include "shortcutcommand.h"
#include <QObject>
#include <QVariant>
#include <KLocalizedString>
#include "../../../SimonLib/EventSimulation/eventhandler.h"

const QString ShortcutCommand::staticCategoryText()
{
	return i18n("Tastenkürzel");
}

const QString ShortcutCommand::getCategoryText() const
{
	return ShortcutCommand::staticCategoryText();
}

const KIcon ShortcutCommand::staticCategoryIcon()
{
	return KIcon("go-jump-locationbar");
}

const KIcon ShortcutCommand::getCategoryIcon() const
{
	return ShortcutCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ShortcutCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;

	out.insert(i18n("Tastenkürzel"), shortcut.toString());
	return out;
}

bool ShortcutCommand::triggerPrivate()
{
	EventHandler::getInstance()->sendShortcut(getShortcut());
	return true;
}
