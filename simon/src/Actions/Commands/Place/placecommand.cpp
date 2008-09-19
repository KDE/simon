#include "placecommand.h"
#include <QObject>
#include <QDesktopServices>
#include <QVariant>
#include <KLocalizedString>


const QString PlaceCommand::staticCategoryText()
{
	return i18n("Ort");
}

const QString PlaceCommand::getCategoryText() const
{
	return PlaceCommand::staticCategoryText();
}

const KIcon PlaceCommand::staticCategoryIcon()
{
	return KIcon("folder");
}

const KIcon PlaceCommand::getCategoryIcon() const
{
	return PlaceCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> PlaceCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("URL"), getURL());
	return out;
}

bool PlaceCommand::triggerPrivate()
{
	return QDesktopServices::openUrl(getURL());
}
