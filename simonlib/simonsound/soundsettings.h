/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SOUNDSETTINGS_H_D0C3C07429B04F65935956A32C796D79
#define SIMON_SOUNDSETTINGS_H_D0C3C07429B04F65935956A32C796D79

#include "ui_deviceconfiguration.h"
#include "ui_postprocessing.h"
#include "ui_vad.h"
#include "ui_trainingsettings.h"
#include "ui_prompt.h"
#include <QVariantList>
#include <QFont>
#include <KConfigWidgets/KCModule>
#include "simonsound_export.h"

class SimonSpinBox;
class QComboBox;
class QCheckBox;
class QTabWidget;
class KFontChooser;
class DeviceSettings;
class SampleGroupContextSettings;

/**
 * \class SoundSettings
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
    DeviceSettings *deviceSettings;
    Ui::VADDlg vadUi;
    Ui::PostProcessing postProcUi;
    Ui::TrainingSettings trainingSettignsUi;
    //	Ui::Prompt promptUi;

    SampleGroupContextSettings* m_sampleGroupContext;

  public slots:
    void load();
    void save();

  private slots:
    void slotChanged();

  public:
    explicit SoundSettings(QWidget* parent, const QVariantList& args=QVariantList());
    ~SoundSettings();

};
#endif
