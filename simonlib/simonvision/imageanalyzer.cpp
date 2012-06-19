#include "imageanalyzer.h"
#include "webcamdispatcher.h"

ImageAnalyzer::ImageAnalyzer(QObject* parent): QObject(parent)
{
    WebcamDispatcher::registerAnalyzer(this);
}


ImageAnalyzer::~ImageAnalyzer()
{
    WebcamDispatcher::unregisterAnalyzer(this);
}
