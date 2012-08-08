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
  virtual ~LipAnalyzer();
  void analyze(IplImage* currentImage);

signals:
  void lipMovementChanged(bool hasLipMovedNew);

private:
  void closeLipDetection();
  int initLipDetection(const QString& faceHaarCascadePath, const QString& lipHaarCascadePath);
  void isChanged(bool hasLipMoved);
  bool hasLipMoved;
  IplImage  * liveVideoFrameCopy;
  IplImage  * prevVideoFrame;
  CvHaarClassifierCascade * faceCascade;
  CvHaarClassifierCascade * lipCascade;
  CvMemStorage * memoryStorage;             // memory for detector to use

};

#endif // LIPANALYZER_H

