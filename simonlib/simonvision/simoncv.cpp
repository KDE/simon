#include "simoncv.h"
#include <KDE/KDebug>

SimonCV::SimonCV(QObject* parent): QObject(parent)
{

}

void SimonCV::detectFace(const QString& cascadeName)
{
    setHaarCascadeXML(cascadeName);

}

SimonCV::~SimonCV()
{

}
void SimonCV::setHaarCascadeXML(const QString& cascadeName)
{
    if (cascade)
        cvReleaseHaarClassifierCascade(&cascade);
    cascade = (CvHaarClassifierCascade *) cvLoad(cascadeName.toUtf8());

}
