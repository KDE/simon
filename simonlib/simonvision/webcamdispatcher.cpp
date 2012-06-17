#include "webcamdispatcher.h"
#include <KDE/KDebug>
#include<QThread>
#include<QTimer>
#include<iostream>

CvCapture* capture=0;
IplImage* liveFrame=0;
QList<ImageAnalyzer*> WebcamDispatcher::analyzers = QList<ImageAnalyzer*>();
WebcamDispatcher* WebcamDispatcher::instance = new WebcamDispatcher;
bool WebcamDispatcher::shouldBeRunning = true;


void WebcamDispatcher::initWebcamDispatcher()
{

    // Initialize video capture
     capture = cvCaptureFromCAM( CV_CAP_ANY );
//    capture = cvCaptureFromCAM( 0 );
    if ( !capture )
    {
        kDebug() << "Failed to initialize video capture\n ";
    }


    return;
}

void WebcamDispatcher::closeWebcamDispatcher()
{
    // Terminate video capture and free capture resources
    cvReleaseCapture( &capture );
    return;
}
void WebcamDispatcher::registerAnalyzer(ImageAnalyzer* analyzer)
{

    WebcamDispatcher::analyzers.append(analyzer);
    if (WebcamDispatcher::analyzers.count()==1)
    {
        WebcamDispatcher::shouldBeRunning = true;
        instance->start();
    }

}

void WebcamDispatcher::unregisterAnalyzer(ImageAnalyzer* analyzer)
{
    // Setting the AnalyzerId of the last analyzer in the QList with the Analyzer to be unregistered
    // This will keep the index at QList and AnalyzerId same
//     WebcamDispatcher::analyzers.at(analyzers.count()-1)->setAnalyzerId(analyzer->getAnalyzerId());

//     WebcamDispatcher::analyzers.removeAt(analyzer->getAnalyzerId());
    for (int i=0;i<WebcamDispatcher::analyzers.count();i++)
    {
        if (analyzer==analyzers.at(i))
        {
            analyzers.removeAt(i);
            std::cout<<"Count:"<<analyzers.count()<<"\n";
        }

    }

    if (WebcamDispatcher::analyzers.count()==0)
    {
        shouldBeRunning = false;
        closeWebcamDispatcher();
    }

}

IplImage* WebcamDispatcher::nextVideoFrame()
{
    liveFrame = cvQueryFrame( capture );

    // If we couldn't grab a frame... quit
    if ( !liveFrame ) {
        kDebug() << "Failed to get the live video frame\n";
        return NULL;
    }
//    cvFlip( liveFrame, 0, 1 );



    return liveFrame;
}


void WebcamDispatcher::run()
{
    initWebcamDispatcher();

    while (shouldBeRunning)
    {
        foreach(ImageAnalyzer* analyzer,analyzers)
        {
            analyzer->analyze(nextVideoFrame());
        }
    }
}

WebcamDispatcher::~WebcamDispatcher()
{

}



