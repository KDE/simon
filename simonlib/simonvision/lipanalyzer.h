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

#ifndef LIPANALYZER_H
#define LIPANALYZER_H
#include "simonvision_export.h"
#include "imageanalyzer.h"

class SIMONVISION_EXPORT LipAnalyzer : public ImageAnalyzer
{
  Q_OBJECT

public:

  LipAnalyzer();
  LipAnalyzer(int thresholdValue);
  virtual ~LipAnalyzer();
  void analyze(const IplImage* currentImage);

signals:
  void lipMovementChanged(bool hasLipMovedNew);

private:
  void closeLipDetection();
  bool initLipDetection(int thresholdValue=40000);
  void isChanged(bool hasLipMoved);
  bool hasLipMoved;
  IplImage  * liveVideoFrameCopy;
  IplImage  * prevVideoFrame;

  //totalCount will count the number of lip movements in last few frames
  //so that If it is more than certain limit then that will mean the person is speaking
  int totalCount;

  int thresholdValue;
  CvHaarClassifierCascade * faceCascade;

  CvHaarClassifierCascade * lipCascade;

  // Memory that will needed to perform detection
  CvMemStorage * memoryStorage;

};

#endif // LIPANALYZER_H

