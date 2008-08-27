#include "textmacrocommand.h"
#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include <KLocalizedString>
#include "../../SimonLib/EventSimulation/eventhandler.h"


const QString TextMacroCommand::staticCategoryText()
{
	return i18n("Text-Makro");
}

const QString TextMacroCommand::getCategoryText() const
{
	return TextMacroCommand::staticCategoryText();
}

const KIcon TextMacroCommand::staticCategoryIcon()
{
	return KIcon("format-text-bold");
}

const KIcon TextMacroCommand::getCategoryIcon() const
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
