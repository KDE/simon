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

#ifndef SIMON_SSC_SAMPLE_H_1E25B8B7C2B84348A22343E8C9A7D679
#define SIMON_SSC_SAMPLE_H_1E25B8B7C2B84348A22343E8C9A7D679

#include <QString>
#include "sscobject.h"
#include "sscobjects_export.h"

class SSCOBJECTS_EXPORT Sample : public SSCObject {

private:
	qint32 m_sampleId;
	qint32 m_typeId;
	qint32 m_userId;
	qint16 m_microphoneId;
	qint16 m_soundCardId;
	QString m_path;
	QString m_prompt;
	QByteArray m_data;

public:
	Sample(qint32 sampleId, qint32 typeId, qint32 userId, qint16 microphoneId, 
			qint16 soundCardId, const QString& path, const QString& prompt,
			const QByteArray& data);

	Sample() {}

	void deserialize(QByteArray data);
	QByteArray serialize();

	qint32 sampleId() { return m_sampleId; }
	qint32 typeId() { return m_typeId; }
	qint32 userId() { return m_userId; }
	qint16 microphoneId() { return m_microphoneId; }
	qint16 soundCardId() { return m_soundCardId; }
	QString path() { return m_path; }
	QString prompt() { return m_prompt; }
	QByteArray data() { return m_data; }

	void setId(qint32 sampleId) { m_sampleId = sampleId; }
	void setPath(const QString& path) { m_path = path; }
	bool deleteFile();

	~Sample() {}
};

#endif



