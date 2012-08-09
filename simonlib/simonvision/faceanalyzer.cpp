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

#include "faceanalyzer.h"
#include "webcamdispatcher.h"
#include "simoncv.h"
#include <KDebug>
#include <KStandardDirs>


using namespace SimonCV;

FaceAnalyzer::FaceAnalyzer()
{
  if (!initFaceDetection(KStandardDirs::locate("data", "haarcascade_frontalface_default.xml")))
    kDebug() <<"Error finding haarcascade_frontalface_default.xml file";
}

int FaceAnalyzer::initFaceDetection(const QString& haarCascadePath)
{
  if (!(memoryStorage = cvCreateMemStorage(0)))
  {
    kDebug() <<"Can\'t allocate memory for face detection\n";
    return 0;
  }

  cascade = (CvHaarClassifierCascade*) cvLoad(haarCascadePath.toUtf8(), 0, 0, 0);

  if (!cascade)
  {
    kDebug() <<"Can\'t load Haar classifier cascade from "<<haarCascadePath<<
    "\nPlease check that this is the correct path\n";
    return 0;
  }

  return 1;
}



void FaceAnalyzer::analyze(IplImage* currentImage)
{

  if (!currentImage)
    return;

  CvRect * faceRect = 0;

//    if (!liveVideoFrameCopy)
  liveVideoFrameCopy = cvCreateImage(cvGetSize(currentImage), 8, 3);

  cvCopy(currentImage, liveVideoFrameCopy, 0);

  // Copy it to the display image, inverting it if needed
  //   pVideoFrameCopy->origin = currentImage->origin;
  //
  //   if ( 1 == pVideoFrameCopy->origin ) // 1 means the image is inverted
  //   {
  //     cvFlip ( pVideoFrameCopy, 0, 0 );
  //     pVideoFrameCopy->origin = 0;
  //   }
  faceRect = detectObject(liveVideoFrameCopy,cascade,memoryStorage);


  if (faceRect)
    isChanged(true);
  else
    isChanged(false);
}


void FaceAnalyzer::closeFaceDetection()
{
  if (cascade)
    cvReleaseHaarClassifierCascade(&cascade);

  if (memoryStorage)
    cvReleaseMemStorage(&memoryStorage);

  if (liveVideoFrameCopy)
    cvReleaseImage(&liveVideoFrameCopy);
}



void FaceAnalyzer::isChanged(bool hasFaceNew)
{
  if (!hasFace == hasFaceNew)
  {
    emit facePresenceChanged(hasFaceNew);
  }

  hasFace = hasFaceNew;
}

FaceAnalyzer::~FaceAnalyzer()
{
  kDebug()<<"Destroying Face Analyzer";

  // Release resources allocated in the analyzer
  closeFaceDetection();
}
