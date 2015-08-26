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

#include <KCoreAddons/KPluginFactory>
#include <simonvision/webcamdispatcher.h>
#include <simonvision/simoncv.h>
#include <QPixmap>
#include "simonwebcamconfiguration.h"
#include "webcamconfiguration.h"

K_PLUGIN_FACTORY( SimonWebcamConfigurationFactory,
                  registerPlugin< SimonWebcamConfiguration >() ; )

// // K_EXPORT_PLUGIN( SimonWebcamConfigurationFactory("SimonWebcamConfiguration"); )

CvCapture* capture=0;

SimonWebcamConfiguration::SimonWebcamConfiguration(QWidget* parent, const QVariantList& args)
    : KCModule(parent)
{
  ui.setupUi(this);
  Q_UNUSED(args);
  addConfig(WebcamConfiguration::self(), this);
  timer = new QTimer(this);
  webcamIndex=WebcamConfiguration::webcamIndex();
  analyzer=0;

  connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
  connect(ui.kcfg_fps, SIGNAL(valueChanged(int)), this, SLOT(displaySliderValue(int)));
  connect(ui.pbNext, SIGNAL(clicked()), this, SLOT(nextWebcam()));
  connect(ui.pbPrev, SIGNAL(clicked()), this, SLOT(prevWebcam()));

  displaySliderValue(WebcamConfiguration::fps());
  startWebcam(webcamIndex);
}

void SimonWebcamConfiguration::displaySliderValue(int value)
{
  ui.lblSilderValue->setText(QString::number(value));
}

void SimonWebcamConfiguration::slotChanged()
{
  emit changed(true);
}
void SimonWebcamConfiguration::updateImage()
{
  QImage *image=SimonCV::IplImage2QImage(cvQueryFrame(capture));
  if(image)
    ui.lblWebcamDisplay->setPixmap(QPixmap::fromImage(*image));
  else
    ui.lblWebcamDisplay->setText("Webcam found but may be another application is using it");
}

void SimonWebcamConfiguration::updateImage(const QImage &image)
{
  if(!image.isNull())
    ui.lblWebcamDisplay->setPixmap(QPixmap::fromImage(image));
  else
    ui.lblWebcamDisplay->setText("Webcam found but may be another application is using it");
}

int SimonWebcamConfiguration::startWebcam(int webcamIndex)
{
  if(webcamIndex<0)
    return 0;
  if(webcamIndex==WebcamConfiguration::webcamIndex())
  {
    emit changed(false);
    timer->stop();
    if(capture)
      cvReleaseCapture(&capture);
    analyzer = new WebcamConfigurationAnalyzer();
    connect(analyzer, SIGNAL(sendImage(QImage)), this, SLOT(updateImage(QImage)));
  }
  else if (capture)
  {
    CvCapture* tempCapture=cvCaptureFromCAM(webcamIndex);
    if(tempCapture)
    {
      CvCapture* tempCapture2=capture;
      capture = tempCapture;
      cvReleaseCapture(&tempCapture2);
    }
    else
    {
      return 0;
    }
  }
  else
  {
    capture=cvCaptureFromCAM(webcamIndex);
    if(!capture)
    {
      return 0;
    }
    if(analyzer)
      delete analyzer;
    analyzer=0;
    timer->start(200);
  }
  return 1;
}



void SimonWebcamConfiguration::prevWebcam()
{
  if(startWebcam(--webcamIndex))
  {
    emit changed(true);
    ui.pbPrev->setEnabled(true);

  }
  else
  {
    webcamIndex++;
    ui.pbPrev->setEnabled(false);
  }
}

void SimonWebcamConfiguration::nextWebcam()
{
  if(startWebcam(++webcamIndex))
  {
    emit changed(true);
    ui.pbNext->setEnabled(true);
  }
  else
  {
    webcamIndex--;
    ui.pbNext->setEnabled(false);
  }
}

void SimonWebcamConfiguration::load()
{
  WebcamConfiguration::self()->readConfig();
  KCModule::load();
}


void SimonWebcamConfiguration::save()
{
  bool isWebcamIndexChanged=bool((WebcamConfiguration::webcamIndex())!=webcamIndex);
  WebcamConfiguration::setWebcamIndex(this->webcamIndex);
  KCModule::save();
  WebcamConfiguration::self()->writeConfig();
  if (capture)
  {
    cvReleaseCapture(&capture);
  }
  if(isWebcamIndexChanged)
  {
    timer->stop();
    WebcamDispatcher::reread(true);
    startWebcam(WebcamConfiguration::webcamIndex());
  }
  else
  {
    WebcamDispatcher::reread(false);
  }
  capture=0;
}

SimonWebcamConfiguration::~SimonWebcamConfiguration()
{
  timer->stop();
  if(capture)
    cvReleaseCapture(&capture);
  if(analyzer)
    delete analyzer;
}

#include "simonwebcamconfiguration.moc"
