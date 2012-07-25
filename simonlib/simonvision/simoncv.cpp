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
#include "simoncv.h"
namespace SimonCV{ 
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
}
