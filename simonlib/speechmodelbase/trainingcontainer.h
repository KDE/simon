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


#ifndef TRAININGCONTAINER_H
#define TRAININGCONTAINER_H

#include "speechmodelbase_export.h"

#include <QByteArray>
#include <QHash>

class SPEECHMODELBASE_EXPORT TrainingContainer
{
	private:
		int m_sampleRate, m_channels;
		QByteArray m_wavConfig;
		QByteArray m_prompts;

	public:
		TrainingContainer(int sampleRate, int channels, const QByteArray& wavConfig,
				const QByteArray& prompts);
			       
		~TrainingContainer();
		
		int sampleRate() { return m_sampleRate; }
		int channels() { return m_channels; }
		QByteArray wavConfig() { return m_wavConfig; }
		QByteArray prompts() { return m_prompts; }
		
};

#endif
