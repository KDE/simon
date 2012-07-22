#ifndef SIMONCV_H
#define SIMONCV_H

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
using namespace cv;

namespace SimonCV
{
  CvRect * detectObject(IplImage * imageFeed, CvHaarClassifierCascade * cascade, CvMemStorage * memoryStorage)
  {
    CvSeq * objectRectSeq=0;                    // memory-access interface    
    CvRect* objectRect = 0;
    if (cascade&&memoryStorage)
    {
      // detect faces in image
      int minObjectSize = imageFeed->width / 5;
      objectRectSeq = cvHaarDetectObjects
      (imageFeed, cascade, memoryStorage,
       1.1,                       // increase search scale by 10% each pass
       6,                         // require six neighbors
       CV_HAAR_DO_CANNY_PRUNING,  // skip regions unlikely to contain a face
       cvSize(minObjectSize, minObjectSize));
      // if one or more faces are detected, return the first one
      
      if (objectRectSeq && objectRectSeq->total)
        objectRect = (CvRect*) cvGetSeqElem(objectRectSeq, 0);
    }
    return objectRect;
  }

};

#endif // SIMONCV_H





