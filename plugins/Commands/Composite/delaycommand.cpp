/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "delaycommand.h"
#include <unistd.h>
#include <QObject>
#include <QVariant>
#include <QDomDocument>
#include <QDomElement>
#include <KIcon>
#include <KLocalizedString>


const QString DelayCommand::staticCategoryText()
{
	return i18n("Delay");
}

const QString DelayCommand::getCategoryText() const
{
	return DelayCommand::staticCategoryText();
}

const KIcon DelayCommand::staticCategoryIcon()
{
	return KIcon("chronometer");
}

const KIcon DelayCommand::getCategoryIcon() const
{
	return DelayCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> DelayCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Delay"), delay);
	return out;
}

bool DelayCommand::triggerPrivate()
{
	usleep(delay*1000);
	return true;
}

QDomElement DelayCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
	QDomElement delayElem = doc->createElement("delay");
	delayElem.appendChild(doc->createTextNode(QString::number(delay)));
	
	commandElem.appendChild(delayElem);
		
	return commandElem;
}

bool DelayCommand::deSerializePrivate(const QDomElement& commandElem)
{
	QDomElement delayElem = commandElem.firstChildElement("delay");
	if (delayElem.isNull()) return false;

	bool ok = true;
	delay = delayElem.text().toInt(&ok);
	
	return ok;
}

STATIC_CREATE_INSTANCE_C(DelayCommand);

