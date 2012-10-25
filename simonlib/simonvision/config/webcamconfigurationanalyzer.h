/*
 *   Copyright (C) 2012 Yash Shah <mail@yashshah.com>
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

#ifndef WEBCAMCONFIGURATIONANALYZER_H
#define WEBCAMCONFIGURATIONANALYZER_H
#include <simonvision/imageanalyzer.h>
#include<QImage>

class WebcamConfigurationAnalyzer : public ImageAnalyzer
{
  Q_OBJECT

public:

  WebcamConfigurationAnalyzer();
  virtual ~WebcamConfigurationAnalyzer();
  void analyze(const IplImage* currentImage);

private:
  int      initFaceDetection(const QString& haarCascadePath);
  void     closeFaceDetection();
  IplImage  * liveVideoFrameCopy;

signals:
   void sendImage(QImage image);

};

#endif // FACEANALYZER_H

