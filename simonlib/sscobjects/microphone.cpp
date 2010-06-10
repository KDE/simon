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

#include "microphone.h"
#include <QDataStream>
#include <QByteArray>

Microphone::Microphone(qint32 id, const QString& model, const QString& type) :
	m_id(id),
	m_model(model),
	m_type(type)
{
}

void Microphone::deserialize(QByteArray data)
{
	QDataStream stream(&data, QIODevice::ReadOnly);
	stream >> m_id;
	stream >> m_model;
	stream >> m_type;
}

QByteArray Microphone::serialize()
{
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);

	bodyStream << m_id;
	bodyStream << m_model;
	bodyStream << m_type;

	return body;
}

