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


#ifndef SOUNDSETTINGS_H
#define SOUNDSETTINGS_H

#include "simonsound_export.h"
#include "sounddevice.h"
#include <QPointer>
#include <KCModule>
#include <QVariantList>
#include <KSharedConfig>
#include <QStringList>
#include <QFont>

class SoundControl;
class SimonSpinBox;
class QComboBox;
class KEditListBox;
class QCheckBox;
class QTabWidget;
class KFontChooser;

/**
 * \class SoundSettinsg
 * \author Peter Grasch
 * \date 12.8.2007
 * \brief SystemWidget to manager the soundsettings
 */
class SIMONSOUND_EXPORT SoundSettings : public KCModule
{
Q_OBJECT

signals:
	void promptFontChanged(const QFont&);

private:
	SimonSpinBox *sbSoundSampleRate;
	QComboBox *cbSoundInputDevice, *cbSoundOutputDevice, *cbSoundChannels;
	QCheckBox *cbProcessInternal;
	KEditListBox *elwProcessingFilters;
	KFontChooser *fontChooser;
	
	static QPointer<SoundSettings> instance;

	SoundControl *sc;

	KSharedConfig::Ptr config;
	bool isChanged;

	int storedInputDevice;
	int storedOutputDevice;
	int storedChannels;
	int storedSampleRate;
	
	bool storedProcessInternal;
	QStringList storedPostProcessingFilters;
	QFont storedPromptFont;


public slots:
	void load();
	void save();
	void defaults();

private slots:
	bool check();
	void checkWithSuccessMessage();
	void slotChanged();
	
public:
	static SoundSettings* getInstance(QWidget *parent=0) {
		if (!instance) instance = new SoundSettings(parent);
		return instance;
	}

	SoundSettings(QWidget* parent, const QVariantList& args=QVariantList());
	~SoundSettings();
	
	int sampleRate() { return storedSampleRate; }
	int channels() { return storedChannels; }
	int inputDevice() { return storedInputDevice; }
	int outputDevice() { return storedOutputDevice; }
	
	bool processInternal() { return storedProcessInternal; }
	QStringList processingFilters() { return storedPostProcessingFilters; }

	QFont promptFont() { return storedPromptFont; }

};

#endif
