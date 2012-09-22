
/*

 *   Copyright (C) 2012 Yash Shah <blazonware@gmail.com>
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
#include <KDebug>
#include<QThread>
#include <QDateTime>
#include<highgui.h>
#include<cv.h>

#define FRAME_PER_SECONDS 10

using namespace cv;

CvCapture* capture=0;
QList<ImageAnalyzer*> analyzers = QList<ImageAnalyzer*>();
WebcamDispatcher* WebcamDispatcher::instance = new WebcamDispatcher;



void WebcamDispatcher::initWebcamDispatcher()
{
  kDebug() << "Initializing webcam dispatcher";
//  cvNamedWindow("Live",1);
  // Initialize video capture
  capture = cvCaptureFromCAM(CV_CAP_ANY);

  if (!capture)
  {
    kDebug() << "Failed to initialize video capture\n ";
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

  kDebug() << "Webcam Dispatcher closed!\n ";
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
    IplImage* liveFrame = cvQueryFrame(capture);

    if (!liveFrame)
    {
      kDebug() << "Failed to get the live video frame\n";
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
    qint64 timeToWait = qMax(qint64(0), 1000 / FRAME_PER_SECONDS - msecsSpentProcessing);
    
    msleep(timeToWait);
  }


}


WebcamDispatcher::~WebcamDispatcher()
{

}
