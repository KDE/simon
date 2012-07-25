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

#include "lipanalyzer.h"
#include<KDebug>
#include "webcamdispatcher.h"
#include "simoncv.h"
using namespace SimonCV;
//// Constants
#define OPENCV_ROOT  ""


LipAnalyzer::LipAnalyzer()
{
    if (!initLipDetection(OPENCV_ROOT
                           "haarcascade_mcs_mouth.xml"))
        kDebug() <<"Error finding haarcascade_mcs_mouth.xml file";
}

int LipAnalyzer::initLipDetection(const char* haarCascadePath)
{
  if (!( memoryStorage = cvCreateMemStorage(0)))
  {
    kDebug() <<"Can\'t allocate memory for lip detection\n";
    return 0;
  }
  
  cascade = (CvHaarClassifierCascade*) cvLoad(haarCascadePath, 0, 0, 0);
  
  if (!cascade)
  {
    kDebug() <<"Can\'t load Haar classifier cascade from "<<haarCascadePath<<
    "\nPlease check that this is the correct path\n";
    return 0;
  }
  
  return 1;
}




void LipAnalyzer::analyze(IplImage* currentImage)
{
    if (!currentImage)
        return;

    CvRect * lipRect = 0;

    // Capture and display video frames until a lip
    // is detected

    // Look for a lip in the next video frame

    if (!liveVideoFrameCopy)
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

    lipRect = detectObject(liveVideoFrameCopy,cascade,memoryStorage);


    if (lipRect)
        isChanged(true);
    else
        isChanged(false);
}


void LipAnalyzer::closeLipDetection()
{
  //    WebcamDispatcher::unregisterAnalyzer(this);
  if (cascade)
    cvReleaseHaarClassifierCascade(&cascade);
  
  if (memoryStorage)
    cvReleaseMemStorage(&memoryStorage);
  
  if (liveVideoFrameCopy)
    cvReleaseImage(&liveVideoFrameCopy);
}



void LipAnalyzer::isChanged(bool hasLipMovedNew)
{
    if (!hasLipMoved == hasLipMovedNew)
    {
        emit lipMovementChanged(hasLipMovedNew);
    }

    hasLipMoved = hasLipMovedNew;
    //kDebug()<<hasLipMoved;
}

LipAnalyzer::~LipAnalyzer()
{
    kDebug()<<"Destroying Lip Analyzer";

    // Release resources allocated in the analyzer
    closeLipDetection();
}
