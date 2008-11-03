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

#include "sounddevice.h"
#include "ui_deviceconfiguration.h"
#include "ui_postprocessing.h"
#include "ui_prompt.h"
#include <KCModule>
#include <QVariantList>
#include <QStringList>
#include <QFont>
#include "simonsound_export.h"

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
	Ui::DeviceConfiguration deviceUi;
	Ui::PostProcessing postProcUi;
	Ui::Prompt promptUi;

	SoundControl *sc;
	int getSelectedInputDeviceId();
	int getSelectedOutputDeviceId();

public slots:
	void load();
	void save();

private slots:
	bool check();
	void checkWithSuccessMessage();
	void slotChanged();
	
public:
	SoundSettings(QWidget* parent, const QVariantList& args=QVariantList());
	~SoundSettings();

};

#endif
