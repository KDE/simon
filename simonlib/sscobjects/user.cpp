/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "user.h"
#include <QDataStream>
#include <QByteArray>
#include <QDebug>

User::User(qint32 userId,QString surname,
		QString givenName,QChar sex,
		qint16 birthYear,QString zipCode,
		QString education,QString currentOccupation,
		QString motherTongueId,QString motherTongue,
		QString diagnosis,qint8 orientation,
		qint8 motorFunction,qint8 communication,
		QString mouthMotoric,
		bool interviewPossible,bool repeatingPossible) :
	m_userId(userId),
	m_surname(surname),
	m_givenName(givenName),
	m_sex(sex),
	m_birthYear(birthYear),
	m_zipCode(zipCode),
	m_education(education),
	m_currentOccupation(currentOccupation),
	m_motherTongueId(motherTongueId),
	m_motherTongue(motherTongue),
	m_diagnosis(diagnosis),
	m_orientation(orientation),
	m_motorFunction(motorFunction),
	m_communication(communication),
	m_mouthMotoric(mouthMotoric),
	m_interviewPossible(interviewPossible),
	m_repeatingPossible(repeatingPossible)
{
}

void User::deserialize(QByteArray data)
{
	QDataStream stream(&data, QIODevice::ReadOnly);
	stream >> m_userId;
	stream >> m_surname;
	stream >> m_givenName;
	stream >> m_sex;
	stream >> m_birthYear;
	stream >> m_zipCode;
	stream >> m_education;
	stream >> m_currentOccupation;
	stream >> m_motherTongueId;
	stream >> m_motherTongue;
	stream >> m_diagnosis;
	stream >> m_orientation;
	stream >> m_motorFunction;
	stream >> m_communication;
	stream >> m_mouthMotoric;
	stream >> m_interviewPossible;
	stream >> m_repeatingPossible;
}

QByteArray User::serialize()
{
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);

	bodyStream << m_userId;
	bodyStream << m_surname;
	bodyStream << m_givenName;
	bodyStream << m_sex;
	bodyStream << m_birthYear;
	bodyStream << m_zipCode;
	bodyStream << m_education;
	bodyStream << m_currentOccupation;
	bodyStream << m_motherTongueId;
	bodyStream << m_motherTongue;
	bodyStream << m_diagnosis;
	bodyStream << m_orientation;
	bodyStream << m_motorFunction;
	bodyStream << m_communication;
	bodyStream << m_mouthMotoric;
	bodyStream << m_interviewPossible;
	bodyStream << m_repeatingPossible;

	qDebug() << "User has been serialized: " << body;
	return body;
}



