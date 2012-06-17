#ifndef FACEANALYZER_H
#define FACEANALYZER_H
#include "simonvision_export.h"
#include "imageanalyzer.h"

class SIMONVISION_EXPORT FaceAnalyzer : public ImageAnalyzer
{
    Q_OBJECT

public:
    FaceAnalyzer();
    void analyze(IplImage* currentImage);
    
signals:
    void facePresenceChanged(bool hasFace);

private:
    bool hasFace;
    bool isChanged(bool hasFace);
/*
    // Parameter settings
    void setVmin(int vmin);
    void setSmin(int smin);

    // Main Control functions
    int     createTracker(const IplImage * pImg);
    void    releaseTracker();
    void    startTracking(IplImage * pImg, CvRect * pRect);
    CvBox2D track(IplImage *);
*/
};

#endif // FACEANALYZER_H
