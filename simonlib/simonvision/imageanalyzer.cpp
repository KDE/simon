#include "imageanalyzer.h"
#include "webcamdispatcher.h"

ImageAnalyzer::ImageAnalyzer(QObject* parent): QObject(parent)
{
  WebcamDispatcher::registerAnalyzer(this);
}

void ImageAnalyzer::analyze(IplImage* currentImage)
{

}
ImageAnalyzer::~ImageAnalyzer()
{
  WebcamDispatcher::unregisterAnalyzer(this);
}
