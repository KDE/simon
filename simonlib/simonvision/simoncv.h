#ifndef SIMONCV_H
#define SIMONCV_H

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include<QImage>
#include "simonvision_export.h"
using namespace cv;

namespace SimonCV
{
extern CvRect * detectObject(IplImage * imageFeed, CvHaarClassifierCascade * cascade, CvMemStorage * memoryStorage);
extern SIMONVISION_EXPORT QImage*  IplImage2QImage(IplImage *iplImg);
}


#endif // SIMONCV_H





