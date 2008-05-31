#include "placecommand.h"
#include <QObject>
#include <QDesktopServices>
#include <QVariant>


const QString PlaceCommand::staticCategoryText()
{
	return QObject::tr("Ort");
}

const QString PlaceCommand::getCategoryText() const
{
	return PlaceCommand::staticCategoryText();
}

const QIcon PlaceCommand::staticCategoryIcon()
{
	return QIcon(":/images/icons/folder.svg");
}

const QIcon PlaceCommand::getCategoryIcon() const
{
	return PlaceCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> PlaceCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(tr("URL"), getURL());
	return out;
}

bool PlaceCommand::triggerPrivate()
{
	return QDesktopServices::openUrl(getURL());
}
