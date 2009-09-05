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

#ifndef RECOGNITIONRESULT_H
#define RECOGNITIONRESULT_H

#include <QList>
#include <QString>
#include <QMetaType>
#include "simonrecognitionresult_export.h"


class SIMONRECOGNITIONRESULT_EXPORT RecognitionResult
{
	private:
		QString m_sentence;
		QString m_sampa;
		QString m_sampaRaw;
		QList<float> m_confidenceScores;
	public:
		RecognitionResult(QString sentence, QString sampa, QString sampaRaw, QList<float> confidenceScores)
			: m_sentence(sentence),
			m_sampa(sampa),
			m_sampaRaw(sampaRaw),
			m_confidenceScores(confidenceScores)
		{
		}

		QString sentence() const { return m_sentence; }
		QString sampa() const { return m_sampa; }
		QString sampaRaw() const { return m_sampaRaw; }
		QList<float> confidenceScores() const { return m_confidenceScores; }

		float averageConfidenceScore() const;

		QString toString() const;

		bool matchesTrigger(const QString& trigger);

		void removeTrigger(const QString& trigger);
};

typedef QList<RecognitionResult> RecognitionResultList;

Q_DECLARE_METATYPE(RecognitionResultList)

#endif
