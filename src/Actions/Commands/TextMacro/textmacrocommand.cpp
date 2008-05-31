#include "textmacrocommand.h"
#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include "../../SimonLib/EventSimulation/eventhandler.h"


const QString TextMacroCommand::staticCategoryText()
{
	return tr("Text-Makro");
}

const QString TextMacroCommand::getCategoryText() const
{
	return TextMacroCommand::staticCategoryText();
}

const QIcon TextMacroCommand::staticCategoryIcon()
{
	return QIcon(":/images/icons/format-text-bold.svg");
}

const QIcon TextMacroCommand::getCategoryIcon() const
{
	return TextMacroCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> TextMacroCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(tr("Text"), getText());
	return out;
}


bool TextMacroCommand::triggerPrivate()
{
	QCoreApplication::processEvents();
	EventHandler::getInstance()->sendWord(getText());
	return true;
}
