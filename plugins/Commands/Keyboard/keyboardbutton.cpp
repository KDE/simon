/*
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
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

#include "keyboardbutton.h"
#include <eventsimulation/eventhandler.h>
#include <QString>
#include <QKeySequence>
#include <QDomDocument>

KeyboardButton::KeyboardButton(const QDomElement& element) : KPushButton(0), m_isNull(false)
{
	if (element.isNull()) m_isNull = true;
	else {
		QDomElement triggerShownElem = element.firstChildElement();
		QDomElement triggerElem = triggerShownElem.nextSiblingElement();
		QDomElement typeElem = triggerElem.nextSiblingElement();
		QDomElement valueElem = typeElem.nextSiblingElement();
		if (triggerShownElem.isNull() || triggerElem.isNull() || 
			    valueElem.isNull() || typeElem.isNull()) {
			m_isNull = true;
		} else {
			triggerShown = triggerShownElem.text();
			triggerReal = triggerElem.text();
			bool ok;
			valueType = (Keyboard::ButtonType) typeElem.text().toInt(&ok);
			value = valueElem.text();
			if (!ok) m_isNull = true;
		}
	}
	setupGUI();
}

/* @param  triggerShown, triggerReal, valueType, value  **/
KeyboardButton::KeyboardButton(QString triggerS, QString triggerR, Keyboard::ButtonType vType, QString v)
	: KPushButton(0), triggerShown(triggerS),
	triggerReal(triggerR),
	valueType(vType),
	value(v),
	m_isNull(false)
{
	setupGUI();
}

void KeyboardButton::setupGUI()
{
	setText(triggerShown);
	setToolTip(triggerReal);
//	connect(this, SIGNAL(clicked()), this, SLOT(trigger()));
}

QString KeyboardButton::getTriggerReal()
{
	if (m_isNull) return QString();
	return triggerReal;
}

QString KeyboardButton::getValue()
{
	if (m_isNull) return QString();
	return value;
}

Keyboard::ButtonType KeyboardButton::getValueType()
{
	if (m_isNull) return Keyboard::NullButton;

	return valueType;
}

QString KeyboardButton::getTriggerShown()
{
	if (m_isNull) return QString();
	return triggerShown;
}

bool KeyboardButton::trigger()
{
	if (m_isNull) return false;

	animateClick();

	switch (valueType) {
		case Keyboard::TextButton:
			EventHandler::getInstance()->sendWord(value);
			break;
		case Keyboard::ShortcutButton:
			EventHandler::getInstance()->sendShortcut(QKeySequence(value));
			break;
		default:
			//nothing to do
			return false;
			break;
	}
	return true;
}

QDomElement KeyboardButton::serialize(QDomDocument *doc)
{
	QDomElement buttonElement = doc->createElement("button");
	QDomElement triggerShownElem = doc->createElement("triggerShown");
	triggerShownElem.appendChild(doc->createTextNode(triggerShown));

	QDomElement triggerElem = doc->createElement("trigger");
	triggerElem.appendChild(doc->createTextNode(triggerReal));

	QDomElement typeElem = doc->createElement("type");
	typeElem.appendChild(doc->createTextNode(QString::number((int) valueType)));

	QDomElement valueElem = doc->createElement("value");
	valueElem.appendChild(doc->createTextNode(value));

	buttonElement.appendChild(triggerShownElem);
	buttonElement.appendChild(triggerElem);
	buttonElement.appendChild(typeElem);
	buttonElement.appendChild(valueElem);

	return buttonElement;
}


#include <KDebug>
KeyboardButton::~KeyboardButton()
{
	kDebug() << "Deleting keyboardbutton";
}

