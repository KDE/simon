
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

#include "webcamdispatcher.h"
#include <QDebug>
#include<QThread>
#include <QDateTime>
#include<highgui.h>
#include<cv.h>
#include "webcamconfiguration.h"

using namespace cv;

CvCapture* capture=0;
QList<ImageAnalyzer*> analyzers = QList<ImageAnalyzer*>();
WebcamDispatcher* WebcamDispatcher::instance = new WebcamDispatcher;

void WebcamDispatcher::reread(bool isWebcamIndexChanged)
{
  WebcamConfiguration::self()->readConfig();
  if(isWebcamIndexChanged)
  {
    if(!instance->analyzers.isEmpty())
    {
      qDebug()<<"Is not empty";
      CvCapture* tempCapture=cvCaptureFromCAM(WebcamConfiguration::webcamIndex());
      if(tempCapture)
      {
        instance->mutexCapture.lock();
        CvCapture* tempCapture2=capture;
        capture = tempCapture;
        cvReleaseCapture(&tempCapture2);
        instance->mutexCapture.unlock();
        return;
      }
    }
  }

}

void WebcamDispatcher::initWebcamDispatcher()
{

  qDebug() << "Initializing webcam dispatcher";
  // cvNamedWindow("Live",1);
  // Initialize video capture
  capture = cvCaptureFromCAM(WebcamConfiguration::webcamIndex());

  if (!capture)
  {
    qDebug() << "Failed to initialize video capture\n ";
    return;
  }
  instance->start();
}

void WebcamDispatcher::closeWebcamDispatcher()
{
  // Terminate video capture and free capture resources
  if (capture)
  {
    cvReleaseCapture(&capture);    
  }

  qDebug() << "Webcam Dispatcher closed!\n ";
}


void WebcamDispatcher::registerAnalyzer(ImageAnalyzer* analyzer)
{
  instance->mutex.lock();
  
  instance->analyzers.append(analyzer);

  if (instance->analyzers.count() ==1)
  {
    instance->initWebcamDispatcher();
  }
  instance->mutex.unlock();
}

void WebcamDispatcher::unregisterAnalyzer(ImageAnalyzer* analyzer)
{
  instance->mutex.lock();
  instance->analyzers.removeAll(analyzer);
  
  if (instance->analyzers.isEmpty())
    instance->closeWebcamDispatcher();
  
  instance->mutex.unlock();
}

void WebcamDispatcher::run()
{

  while (instance->analyzers.count()!=0)
  {
    instance->mutexCapture.lock();
    IplImage* liveFrame = cvQueryFrame(capture);
    instance->mutexCapture.unlock();

    if (!liveFrame)
    {
      qDebug() << "Failed to get the live video frame\n";
    }
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    instance->mutex.lock();

    foreach(ImageAnalyzer* analyzer,analyzers)
    {
//    cvShowImage("Testing", nextVideoFrame() );
      analyzer->analyze(liveFrame);
    }

    instance->mutex.unlock();
    
    qint64 msecsSpentProcessing = QDateTime::currentMSecsSinceEpoch() - currentTime;
    qint64 timeToWait = qMax(qint64(0), 1000 / WebcamConfiguration::fps() - msecsSpentProcessing);
    
    msleep(timeToWait);
  }


}


WebcamDispatcher::~WebcamDispatcher()
{

}
