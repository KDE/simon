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

#include "webcamconfigurationanalyzer.h"
#include <simonvision/webcamdispatcher.h>
#include <simonvision/simoncv.h>
#include <QDebug>

WebcamConfigurationAnalyzer::WebcamConfigurationAnalyzer()
{
  liveVideoFrameCopy=0;
}

void WebcamConfigurationAnalyzer::analyze(const IplImage* currentImage)
{
  if (!currentImage)
    return;

  liveVideoFrameCopy = cvCreateImage(cvGetSize(currentImage), 8, 3);

  cvCopy(currentImage, liveVideoFrameCopy, 0);

  // declare a destination IplImage object with correct size, depth and channels
  IplImage *smallImage = cvCreateImage( cvSize(128,128),liveVideoFrameCopy->depth, liveVideoFrameCopy->nChannels );

  //use cvResize to resize source to a destination image
  cvResize(liveVideoFrameCopy, smallImage);
  emit sendImage(*SimonCV::IplImage2QImage(smallImage));

  cvReleaseImage(&smallImage);
  cvReleaseImage(&liveVideoFrameCopy);
}


WebcamConfigurationAnalyzer::~WebcamConfigurationAnalyzer()
{
  qDebug()<<"Destroying Webcam Configuration Analyzer";
  WebcamDispatcher::unregisterAnalyzer(this);

  // Release resources allocated in the analyzer
  if (liveVideoFrameCopy)
    cvReleaseImage(&liveVideoFrameCopy);
}
