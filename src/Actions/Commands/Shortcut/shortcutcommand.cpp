#include "shortcutcommand.h"
#include <QObject>
#include <QVariant>
#include "../../../SimonLib/EventSimulation/eventhandler.h"

const QString ShortcutCommand::staticCategoryText()
{
	return QObject::tr("Tastenkürzel");
}

const QString ShortcutCommand::getCategoryText() const
{
	return ShortcutCommand::staticCategoryText();
}

const QIcon ShortcutCommand::staticCategoryIcon()
{
	return QIcon(":/images/icons/go-jump-locationbar.svg");
}

const QIcon ShortcutCommand::getCategoryIcon() const
{
	return ShortcutCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ShortcutCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	const Shortcut *shortcut = getShortcut();
	if (!shortcut) return out;

	out.insert(tr("Tastenkürzel"), shortcut->getDescription());
	return out;
}

bool ShortcutCommand::triggerPrivate()
{
	EventHandler::getInstance()->sendShortcut(getShortcut());
	return true;
}
