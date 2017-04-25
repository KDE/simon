/*
 *   Copyright (C) 2012 Yash Shah <mail@yashshah.com>
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

#ifndef SIMON_WEBCAMCONFIGURATION_H
#define SIMON_WEBCAMCONFIGURATION_H

#include <QVariantList>
#include <QImage>
#include <cv.h>
#include <highgui.h>
#include "webcamconfigurationanalyzer.h"
#include "ui_simonwebcamconfiguration.h"
#include <QTimer>
#include <KCModule>

class SimonWebcamConfiguration : public KCModule
{
  Q_OBJECT

  private:
    Ui::WebcamConfiguration ui;
    int webcamIndex;
    WebcamConfigurationAnalyzer* analyzer;
    int startWebcam(int webcamIndex);
    QTimer *timer;

  private slots:
    void slotChanged();
    void displaySliderValue(int value);
    void prevWebcam();
    void nextWebcam();
    void updateImage();
    void updateImage(const QImage& image);

  public:
    explicit SimonWebcamConfiguration(QWidget* parent, const QVariantList& args=QVariantList());

    virtual void save();
    virtual void load();
    ~SimonWebcamConfiguration();
};
#endif
