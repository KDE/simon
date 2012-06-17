#include "faceanalyzer.h"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
//// Constants
const char * DISPLAY_WINDOW = "DisplayWindow";
#define OPENCV_ROOT  ""
using namespace std;

//// Global variables
IplImage  * pVideoFrameCopy = 0;


// File-level variables
CvHaarClassifierCascade * pCascade = 0;  // the face detector
CvMemStorage * pStorage = 0;             // memory for detector to use
CvSeq * pFaceRectSeq;                    // memory-access interface



//// Function definitions
void exitProgram(int code);

int      initFaceDet(const char * haarCascadePath);
void     closeFaceDet();
CvRect * detectFace(IplImage * pImg);

FaceAnalyzer::FaceAnalyzer()
{
    if ( !initFaceDet(OPENCV_ROOT
                      "haarcascade_frontalface_default.xml"))
        std::cout<<"Error";


    // Create the display window
    cvNamedWindow( DISPLAY_WINDOW, 1 );
    /*
        // Initialize tracker
        captureVideoFrame();
        if ( !createTracker(pVideoFrameCopy) ) return 0;

        // Set Camshift parameters
        setVmin(60);
        setSmin(50);
    */
}



void FaceAnalyzer::analyze(IplImage* currentImage)
{
    if(!currentImage)
      return;
    CvRect * pFaceRect = 0;

    // Capture and display video frames until a face
    // is detected

    // Look for a face in the next video frame
    // Copy it to the display image, inverting it if needed
    if ( !pVideoFrameCopy )
        pVideoFrameCopy = cvCreateImage( cvGetSize(currentImage), 8, 3 );
    cvCopy( currentImage, pVideoFrameCopy, 0 );
    pVideoFrameCopy->origin = currentImage->origin;

    if ( 1 == pVideoFrameCopy->origin ) // 1 means the image is inverted
    {
        cvFlip( pVideoFrameCopy, 0, 0 );
        pVideoFrameCopy->origin = 0;
    }
    pFaceRect = detectFace(pVideoFrameCopy);

    // Show the display image
    cvShowImage( DISPLAY_WINDOW, pVideoFrameCopy );
    //if ( (char)27==cvWaitKey(1) ) exitProgram(0);

    // exit loop when a face is detected
    if (pFaceRect) {}
    /*
        // initialize tracking
        startTracking(pVideoFrameCopy, pFaceRect);

        // Track the detected face using CamShift
        while ( 1 )
        {
            CvBox2D faceBox;

            // get the next video frame
            captureVideoFrame();

            // track the face in the new video frame
            faceBox = track(pVideoFrameCopy);

            // outline face ellipse
            cvEllipseBox(pVideoFrameCopy, faceBox,
                         CV_RGB(255,0,0), 3, CV_AA, 0 );
            cvShowImage( DISPLAY_WINDOW, pVideoFrameCopy );
            if ( (char)27==cvWaitKey(1) ) break;
        }
    */
}




//////////////////////////////////
// exitProgram()
//
void exitProgram(int code)
{
    // Release resources allocated in this file
    cvDestroyWindow( DISPLAY_WINDOW );
    cvReleaseImage( &pVideoFrameCopy );

    // Release resources allocated in other project files
    closeFaceDet();
//    releaseTracker();

    exit(code);
}





//////////////////////////////////
// initFaceDet()
//
int initFaceDet(const char * haarCascadePath)
{
    char the_path[256];

    getcwd(the_path, 255);
    printf("%s\n", the_path);
    fprintf(stderr, haarCascadePath);
    if ( !(pStorage = cvCreateMemStorage(0)) )
    {
        fprintf(stderr, "Can\'t allocate memory for face detection\n");
        return 0;
    }

    pCascade = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if ( !pCascade )
    {
        fprintf(stderr, "Can\'t load Haar classifier cascade from\n"
                "   %s\n"
                "Please check that this is the correct path\n",
                haarCascadePath);
        return 0;
    }

    return 1;
}


//////////////////////////////////
// closeFaceDet()
//
void closeFaceDet()
{
    if (pCascade)
        cvReleaseHaarClassifierCascade(&pCascade);
    if (pStorage)
        cvReleaseMemStorage(&pStorage);
}


//////////////////////////////////
// detectFace()

CvRect * detectFace(IplImage * pImg)
{
    CvRect* r = 0;

    // detect faces in image
    int minFaceSize = pImg->width / 5;
    pFaceRectSeq = cvHaarDetectObjects
                   (pImg, pCascade, pStorage,
                    1.1,                       // increase search scale by 10% each pass
                    6,                         // require six neighbors
                    CV_HAAR_DO_CANNY_PRUNING,  // skip regions unlikely to contain a face
                    cvSize(minFaceSize, minFaceSize));

    // if one or more faces are detected, return the first one
    if ( pFaceRectSeq && pFaceRectSeq->total )
        r = (CvRect*)cvGetSeqElem(pFaceRectSeq, 0);

    return r;
}







bool FaceAnalyzer::isChanged(bool hasFace)
{
    if (this->hasFace == hasFace)
        return false;
    else
        return true;
}
