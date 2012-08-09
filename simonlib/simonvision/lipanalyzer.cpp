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
#include "webcamdispatcher.h"
#include "simoncv.h"
#include <KDebug>
#include <KStandardDirs>

using namespace SimonCV;

LipAnalyzer::LipAnalyzer()
{
  if (!initLipDetection(KStandardDirs::locate("data", "haarcascade_mcs_mouth.xml"),
                        KStandardDirs::locate("data", "haarcascade_frontalface_default.xml")))
    kDebug() <<"Error finding haarcascade file";

}

bool LipAnalyzer::initLipDetection(const QString& lipHaarCascadePath, const QString& faceHaarCascadePath)
{
  prevVideoFrame=0;

  if (!(memoryStorage = cvCreateMemStorage(0)))
  {
    kDebug() <<"Can\'t allocate memory for lip detection\n";
    return false;
  }

  faceCascade = (CvHaarClassifierCascade*) cvLoad(faceHaarCascadePath.toUtf8(), 0, 0, 0);

  if (!faceCascade)
  {
    kDebug() <<"Can\'t load Haar classifier face cascade from "<<faceHaarCascadePath<<
    "\nPlease check that this is the correct path\n";
    return false;
  }

  lipCascade = (CvHaarClassifierCascade*) cvLoad(lipHaarCascadePath.toUtf8(), 0, 0, 0);

  if (!lipCascade)
  {
    kDebug() <<"Can\'t load Haar classifier lip cascade from "<<lipHaarCascadePath<<
    "\nPlease check that this is the correct path\n";
    return false;
  }

  return true;
}




void LipAnalyzer::analyze(IplImage* currentImage)
{

  if (!currentImage)
    return;

  CvRect * faceRect = 0;


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


  faceRect = detectObject(liveVideoFrameCopy,faceCascade,memoryStorage);


  if (faceRect)
  {
    
//     cvRectangle(liveVideoFrameCopy,cvPoint(faceRect->x, faceRect->y),
//                 cvPoint(faceRect->x + faceRect->width, faceRect->y + faceRect->height),
//                 CV_RGB(255, 0, 0), 1, 8, 0);

    cvSetImageROI(liveVideoFrameCopy,/* the source image */
                  cvRect(faceRect->x,            /* x = start from leftmost */
                         faceRect->y+(faceRect->height *2/3), /* We are just considering the lower part */
                         faceRect->width,        /* width is same as of the face */
                         faceRect->height/3)    /* height is the 1/3 of face height */
                 );
    CvRect * mouthRect = 0;
    mouthRect = detectObject(liveVideoFrameCopy,lipCascade,memoryStorage);

    if (mouthRect)
    {
      cvRectangle(liveVideoFrameCopy,cvPoint(faceRect->x, faceRect->y),
                  cvPoint(faceRect->x + faceRect->width, faceRect->y + faceRect->height),
                  CV_RGB(0, 255, 0), 1, 8, 0);
      cvSetImageROI(liveVideoFrameCopy,
                    cvRect(faceRect->x + mouthRect->x-15,            /* x = start from leftmost */
                           faceRect->y+(faceRect->height *2/3) + mouthRect->y-5, /* y = a few pixels from the top */
                           100,        /* width = same width with the face */
                           55)    /* height = 1/3 of face height */
                   );

      if (!prevVideoFrame)
      {
        prevVideoFrame = cvCreateImage(cvGetSize(liveVideoFrameCopy),liveVideoFrameCopy->depth,liveVideoFrameCopy->nChannels);
      }

      IplImage *diff = cvCreateImage(cvGetSize(liveVideoFrameCopy),liveVideoFrameCopy->depth,liveVideoFrameCopy->nChannels);

      cvAbsDiff(prevVideoFrame,liveVideoFrameCopy,diff);



      int height    = diff->height;

      int width     = diff->width;
      int step      = diff->widthStep;
      int channels  = diff->nChannels;
      uchar* data= (uchar *)diff->imageData;
      int sum = 0;

      for (int i=0;i<height;i++)
      {
        for (int j=0;j<width;j++)
        {
          int gray = 0;

          for (int k=0;k<channels;k++)
          {
            gray += data[i*step+j*channels+k];
          }

          sum+=gray/3;
        }
      }

      cvCopy(liveVideoFrameCopy,prevVideoFrame);

      if (sum>21000)
      {
        isChanged(true);
        kDebug()<<"Speaking: TRUE\n";
      }

      else
      {
        isChanged(false);
        kDebug()<<"Speaking: False\n";

      }
    }



  }

}

void LipAnalyzer::closeLipDetection()
{
  //    WebcamDispatcher::unregisterAnalyzer(this);

  cvReleaseHaarClassifierCascade(&faceCascade);

  if (memoryStorage)
    cvReleaseMemStorage(&memoryStorage);

  cvReleaseImage(&liveVideoFrameCopy);
}



void LipAnalyzer::isChanged(bool hasLipMovedNew)
{
  if (!hasLipMoved == hasLipMovedNew)
  {
    emit lipMovementChanged(hasLipMovedNew);
  }

  hasLipMoved = hasLipMovedNew;
}

LipAnalyzer::~LipAnalyzer()
{
  kDebug()<<"Destroying Lip Analyzer";

  // Release resources allocated in the analyzer
//  closeLipDetection();
}