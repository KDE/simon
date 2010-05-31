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

#include "sample.h"
#include <QDataStream>
#include <QFile>
#include <QByteArray>

Sample::Sample(qint32 sampleId, qint32 typeId, qint32 userId, qint16 microphoneId, 
			qint16 soundCardId, const QString& path, const QString& prompt,
			const QByteArray& data) :
	m_sampleId(sampleId),
	m_typeId(typeId),
	m_userId(userId),
	m_microphoneId(microphoneId),
	m_soundCardId(soundCardId),
	m_path(path),
	m_prompt(prompt),
	m_data(data)
{
}


void Sample::deserialize(QByteArray data)
{
	QDataStream stream(&data, QIODevice::ReadOnly);
	stream >> m_sampleId;
	stream >> m_typeId;
	stream >> m_userId;
	stream >> m_microphoneId;
	stream >> m_soundCardId;
	stream >> m_path;
	stream >> m_prompt;
	stream >> m_data;
}

QByteArray Sample::serialize()
{
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);

	bodyStream << m_sampleId;
	bodyStream << m_typeId;
	bodyStream << m_userId;
	bodyStream << m_microphoneId;
	bodyStream << m_soundCardId;
	bodyStream << m_path;
	bodyStream << m_prompt;
	bodyStream << m_data;

	return body;
}

bool Sample::deleteFile()
{
  return QFile::remove(m_path);
}

