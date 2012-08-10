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
#include<highgui.h>
#include<cv.h>
using namespace cv;

CvCapture* capture=0;
QList<ImageAnalyzer*> analyzers = QList<ImageAnalyzer*>();
WebcamDispatcher* WebcamDispatcher::instance = new WebcamDispatcher;



void WebcamDispatcher::initWebcamDispatcher()
{

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
  kDebug() << "Registering analyzer\n ";

  instance->analyzers.append(analyzer);

  if (instance->analyzers.count() ==1)
  {
    instance->initWebcamDispatcher();
  }

}

void WebcamDispatcher::unregisterAnalyzer(ImageAnalyzer* analyzer)
{
  kDebug() << "Unregistering analyzer\n ";

  for (int i=0;i<instance->analyzers.count();i++)
  {
    if (analyzer==instance->analyzers.at(i))
    {
      instance->analyzers.removeAt(i);
      kDebug() << "Instance removed\n  ";
    }

  }


}

IplImage* WebcamDispatcher::nextVideoFrame()
{
  IplImage* liveFrame = cvQueryFrame(capture);


  if (!liveFrame)
  {
    kDebug() << "Failed to get the live video frame\n";
//       return NULL;
  }

//    cvFlip( liveFrame, 0, 1 );
  return liveFrame;
}


void WebcamDispatcher::run()
{

  while (instance->analyzers.count()!=0)
  {
    foreach(ImageAnalyzer* analyzer,analyzers)

    {
//    cvShowImage("Testing", nextVideoFrame() );
      analyzer->analyze(nextVideoFrame());
    }
  }

    instance->closeWebcamDispatcher();

}


WebcamDispatcher::~WebcamDispatcher()
{

}


