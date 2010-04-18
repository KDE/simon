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


#ifndef SIMON_LOUDNESSMETERSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD
#define SIMON_LOUDNESSMETERSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD

#include "simonsound_export.h"
#include "soundprocessor.h"

class SIMONSOUND_EXPORT LoudnessMeterSoundProcessor : public SoundProcessor {

private:
	int m_peak;
	bool m_clipping;

public:
	LoudnessMeterSoundProcessor();

	void process(QByteArray& data);

	int peak() { return m_peak; }
	bool clipping() { return m_clipping; }
};

#endif



