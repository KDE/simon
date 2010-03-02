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


#ifndef SIMON_TRAININGCONTAINER_H_A1DE97D32A7149D4810017795E704C7C
#define SIMON_TRAININGCONTAINER_H_A1DE97D32A7149D4810017795E704C7C

#include "simonmodelmanagement_export.h"

#include <QByteArray>
#include <QHash>

class MODELMANAGEMENT_EXPORT TrainingContainer
{
	private:
		qint32 m_sampleRate;
		QByteArray m_wavConfig;
		QByteArray m_prompts;

	public:
		TrainingContainer(qint32 sampleRate, const QByteArray& wavConfig,
				const QByteArray& prompts);
			       
		~TrainingContainer();
		
		int sampleRate() { return m_sampleRate; }
		QByteArray wavConfig() { return m_wavConfig; }
		QByteArray prompts() { return m_prompts; }
		
};

#endif
