#ifndef SIMONCV_H
#define SIMONCV_H

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

using namespace cv;

namespace SimonCV
{
extern CvRect * detectObject(IplImage * imageFeed, CvHaarClassifierCascade * cascade, CvMemStorage * memoryStorage);
}


#endif // SIMONCV_H





