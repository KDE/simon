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

#ifndef SIMON_KEYBOARDBUTTON_H_FE27CA2130AA40A68CE748E28ACBE7DA
#define SIMON_KEYBOARDBUTTON_H_FE27CA2130AA40A68CE748E28ACBE7DA

#include <QString>
#include <QDomElement>
#include <KPushButton>

class QDomDocument;

namespace Keyboard {
	enum ButtonType{
		NullButton=0,
		TextButton=1,
		ShortcutButton=2
	};
}

class KeyboardButton : public KPushButton
{
	Q_OBJECT

	private:
		QString value;
		bool m_isNull;
		QString triggerShown;
		QString triggerReal;
		Keyboard::ButtonType valueType;
		void setupGUI();
	
	public slots:
		bool trigger();

	public:
		bool isNull() { return m_isNull; }

		KeyboardButton(QString triggerShown, QString triggerReal, Keyboard::ButtonType valueType, QString value);
		KeyboardButton(const QDomElement& elem);
		~KeyboardButton();

		QString getTriggerReal();
		Keyboard::ButtonType getValueType();
		QString getValue();
		QString getTriggerShown();
		
		void setTriggerShown(const QString& triggerShown);
		void setTriggerReal(const QString& triggerReal);
		void setValue(const QString& value);
		void setButtonType(Keyboard::ButtonType valueType);

		QDomElement serialize(QDomDocument *doc);
};

#endif
