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
#include<KDebug>
#include "webcamdispatcher.h"

//// Constants
#define OPENCV_ROOT  ""

using namespace std;
//// Global variables
IplImage  * pVideoFrameCopy = 0;


// File-level variables
CvHaarClassifierCascade * pCascade = 0;  // the face detector
CvMemStorage * pStorage = 0;             // memory for detector to use
CvSeq * pFaceRectSeq;                    // memory-access interface

int      initFaceDet(const char * haarCascadePath);
void     closeFaceDet();
CvRect * detectFace(IplImage * pImg);

FaceAnalyzer::FaceAnalyzer()
{
  if (!initFaceDet(OPENCV_ROOT
                   "haarcascade_frontalface_default.xml"))
    kDebug() <<"Error finding haarcascade_frontalface_default.xml file";
}

int initFaceDet(const char *haarCascadePath)
{

  if (!(pStorage = cvCreateMemStorage(0)))
  {
    kDebug() <<"Can\'t allocate memory for face detection\n";
    return 0;
  }

  pCascade = (CvHaarClassifierCascade*) cvLoad(haarCascadePath, 0, 0, 0);

  if (!pCascade)
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

  CvRect * pFaceRect = 0;

  // Capture and display video frames until a face
  // is detected

  // Look for a face in the next video frame

  if (!pVideoFrameCopy)
    pVideoFrameCopy = cvCreateImage(cvGetSize(currentImage), 8, 3);

  cvCopy(currentImage, pVideoFrameCopy, 0);

  // Copy it to the display image, inverting it if needed
  //   pVideoFrameCopy->origin = currentImage->origin;
  //
  //   if ( 1 == pVideoFrameCopy->origin ) // 1 means the image is inverted
  //   {
  //     cvFlip ( pVideoFrameCopy, 0, 0 );
  //     pVideoFrameCopy->origin = 0;
  //   }

  pFaceRect = detectFace(pVideoFrameCopy);


  if (pFaceRect)
    isChanged(true);
  else
    isChanged(false);
}


void closeFaceDet()
{
  if (pCascade)
    cvReleaseHaarClassifierCascade(&pCascade);

  if (pStorage)
    cvReleaseMemStorage(&pStorage);
}

CvRect * detectFace(IplImage * pImg)
{
    
  CvRect* r = 0;
  if(pCascade&&pStorage)
  {    
  // detect faces in image
  int minFaceSize = pImg->width / 5;
  pFaceRectSeq = cvHaarDetectObjects
                 (pImg, pCascade, pStorage,
                  1.1,                       // increase search scale by 10% each pass
                  6,                         // require six neighbors
                  CV_HAAR_DO_CANNY_PRUNING,  // skip regions unlikely to contain a face
                  cvSize(minFaceSize, minFaceSize));
  // if one or more faces are detected, return the first one

  if (pFaceRectSeq && pFaceRectSeq->total)
    r = (CvRect*) cvGetSeqElem(pFaceRectSeq, 0);
  }
  return r;
}

void FaceAnalyzer::isChanged(bool hasFaceNew)
{
  if (!hasFace == hasFaceNew)
  {
    emit facePresenceChanged(hasFaceNew);
  }

  hasFace = hasFaceNew;
  //kDebug()<<hasFace;
}

FaceAnalyzer::~FaceAnalyzer()
{
  kDebug()<<"Destroying Face Analyzer";
  WebcamDispatcher::unregisterAnalyzer(this);
  // Release resources allocated in this file
  cvReleaseImage(&pVideoFrameCopy);
  cvDestroyWindow( "Smile" ); 
  
  // Release resources allocated
  closeFaceDet();
}
