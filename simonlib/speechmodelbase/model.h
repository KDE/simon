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


#ifndef MODEL_H
#define MODEL_H


#include "speechmodelbase_export.h"

#include <QString>
#include <QHash>
#include <QStringList>

class SPEECHMODELBASE_EXPORT Model
{
	private:
		int m_sampleRate;
		QByteArray m_hmmDefs;
		QByteArray m_tiedList;
		QByteArray m_dict;
		QByteArray m_dfa;

	public:
		Model(int sampleRate, const QByteArray& hmmDefs,
				const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa);
			       
		~Model();
		
		int sampleRate() { return m_sampleRate; }
		QByteArray hmmDefs() { return m_hmmDefs; }
		QByteArray tiedList() { return m_tiedList; }
		QByteArray dict() { return m_dict; }
		QByteArray dfa() { return m_dfa; }
		
};

#endif
