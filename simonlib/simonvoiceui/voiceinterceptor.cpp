/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "voiceinterceptor.h"
#include <QMetaObject>

VoiceInterceptor::VoiceInterceptor(const QString& id, const QString& trigger, const QString& visibleTrigger, 
			const QString& description) :
	m_id(id), 
	m_trigger(trigger), 
	m_visibleTrigger(visibleTrigger),
	m_description(description)
{
}

bool VoiceInterceptor::deSerialize(const QDomElement& element)
{
	if (element.isNull()) return false;

	QDomElement idElement = element.firstChildElement("id");
	QDomElement triggerElement = element.firstChildElement("trigger");
	QDomElement visibleTriggerElement = element.firstChildElement("visibleTrigger");
	QDomElement descriptionElement = element.firstChildElement("descriptionElement");
	m_id = idElement.text();
	m_trigger = triggerElement.text();
	m_visibleTrigger = visibleTriggerElement.text();
	m_description = descriptionElement.text();

	return true;
}

QDomElement VoiceInterceptor::serialize(QDomDocument *doc)
{
	Q_ASSERT(doc);

	QDomElement elem = doc->createElement("interceptor");

	QDomElement idElement = doc->createElement("id");
	idElement.appendChild(doc->createTextNode(m_id));

	QDomElement triggerElement = doc->createElement("trigger");
	triggerElement.appendChild(doc->createTextNode(m_trigger));

	QDomElement visibleTriggerElement = doc->createElement("visibleTrigger");
	visibleTriggerElement.appendChild(doc->createTextNode(m_visibleTrigger));

	QDomElement descriptionElement = doc->createElement("description");
	descriptionElement.appendChild(doc->createTextNode(m_description));

	elem.appendChild(idElement);
	elem.appendChild(triggerElement);
	elem.appendChild(visibleTriggerElement);
	elem.appendChild(descriptionElement);

	return elem;
}

VoiceInterceptor::~VoiceInterceptor()
{
}

