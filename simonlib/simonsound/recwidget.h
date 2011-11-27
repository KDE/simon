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

#ifndef SIMON_RECWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4
#define SIMON_RECWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4

#include "simonsound_export.h"
#include "simonsamples.h"
#include "simonsound.h"
#include <QWidget>

class WavFileWidget;
class QTimer;

namespace Ui
{
  class RecWidgetUi;
}


/**
 * \class RecWidget
 * \brief This class provides a nice Recording Widget to easily use within the application
 *
 * It draws the GUI and uses the WavRecorderClient/WavPlayerClient classes
 *
 * \author Peter Grasch
 * \date 26.05.2007
 */
class SIMONSOUND_EXPORT RecWidget : public QWidget
{
  Q_OBJECT

  signals:
    void playing();
    void recording();
    void sampleDeleted();
    void progress(int);
    void recordingFinished();
    void playbackFinished();

  private:
    QTimer *statusTimer;
    Ui::RecWidgetUi *ui;
    bool m_simpleMode;
    bool m_playbackOnly;

    QList<WavFileWidget*> waves;

    QString fileTemplate;

    int recordingProgress;

    void setupSignalsSlots();

    //void registerDevice(const QString& id, int channels, int sampleRate, const QString& filenameSuffix);
    void registerDevice(const SimonSound::DeviceConfiguration& device, const QString& filenameSuffix);

    void adjustButtonsToFile();

  private slots:
    void initialize();
    void initialize(QList<SimonSound::DeviceConfiguration> forcedDevices);

    void changePromptFont(const QFont& font);
    void displayError(const QString& error);

    void slotSampleDeleted();

    void hideActionPrompt();
    void showStartPrompt();
    void showFinishPrompt();
    void showWaitPrompt();

  public slots:
    void record();
    void stopRecording();

    void setTitle(QString newTitle);

    bool deleteAll();
    void stopPlayback();
    void slotEnableDeleteAll();

  public:
    RecWidget(QString name, QString text, QString fileTemplate, bool forceSimpleMode=false, 
	              QWidget *parent=0, QList<SimonSound::DeviceConfiguration> forcedDevices = QList<SimonSound::DeviceConfiguration>(),
                bool playbackOnly=false);
    ~RecWidget();
    bool hasRecordingReady();
    bool isRecording();
    QStringList getFileNames();
    QString getSampleGroup();
    QStringList getDevices();

    void checkFile();

    bool isPlaying();
    void play();

    SimonSamples::SampleProblems sampleProblems();

    bool forceSimpleMode() const { return m_simpleMode; }
    QString getFileTemplate() const { return fileTemplate; }

};
#endif
