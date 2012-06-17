#ifndef SIMONCV_H
#define SIMONCV_H

#include "cv.h"
#include "highgui.h"
#include<QObject>
#include<QWidget>
#include <QImage>

#include "cxcore.h"
using namespace cv;

class SimonCV : public QObject
{
    Q_OBJECT

public:
    explicit SimonCV(QObject* parent = 0);
    virtual ~SimonCV();
    void detectFace(const QString& cascadeName);
    void setHaarCascadeXML(const QString& cascadeName);
    QString cascadeFile() const;
private:

    CvHaarClassifierCascade* cascade;
    CvMemStorage *mStorage;

    QString mCascadeFile;
};

#endif // SIMONCV_H

