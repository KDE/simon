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
QImage* qImage=NULL;
namespace SimonCV{ 

  CvRect * detectObject(IplImage * imageFeed, CvHaarClassifierCascade * cascade, CvMemStorage * memoryStorage)
  {
    CvSeq * objectRectSeq=0;                    // memory-access interface
    CvRect* objectRect = 0;
    if (cascade&&memoryStorage)
    {
      //       // detect faces in image
      //       int minObjectSize = imageFeed->width / 5;
      //       objectRectSeq = cvHaarDetectObjects
      //       (imageFeed, cascade, memoryStorage,
      //        1.1,                       // increase search scale by 10% each pass
      //        6,                         // require six neighbors
      //        CV_HAAR_DO_CANNY_PRUNING,  // skip regions unlikely to contain a face
      //        cvSize(40, 40));
      //       // if one or more faces are detected, return the first one
      // detect faces in image
      objectRectSeq = cvHaarDetectObjects
          (imageFeed, cascade, memoryStorage,
           1.1,                       // increase search scale by 10% each pass
           30,                         // require six neighbors
           CV_HAAR_DO_CANNY_PRUNING,  // skip regions unlikely to contain a face
           cvSize(40, 40));
      // if one or more faces are detected, return the first one
      if (objectRectSeq && objectRectSeq->total)
        objectRect = (CvRect*) cvGetSeqElem(objectRectSeq, 0);
    }
    return objectRect;
  }


  QImage* IplImage2QImage(IplImage *iplImg)
  {
    if(iplImg)
    {
      int h = iplImg->height;
      int w = iplImg->width;
      int channels = iplImg->nChannels;
      if(qImage == NULL)
        qImage = new QImage(w, h, QImage::Format_ARGB32);
      char *data = iplImg->imageData;

      for (int y = 0; y < h; y++, data += iplImg->widthStep)
      {
        for (int x = 0; x < w; x++)
        {
          char r, g, b, a = 0;
          if (channels == 1)
          {
            r = data[x * channels];
            g = data[x * channels];
            b = data[x * channels];
          }
          else if (channels == 3 || channels == 4)
          {
            r = data[x * channels + 2];
            g = data[x * channels + 1];
            b = data[x * channels];
          }

          if (channels == 4)
          {
            a = data[x * channels + 3];
            qImage->setPixel(x, y, qRgba(r, g, b, a));
          }
          else
          {
            qImage->setPixel(x, y, qRgb(r, g, b));
          }
        }
      }
      return qImage;

    }
    return new QImage();
  }

}

