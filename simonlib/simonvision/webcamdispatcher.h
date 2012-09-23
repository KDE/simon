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

#ifndef WEBCAMDISPATCHER_H
#define WEBCAMDISPATCHER_H

#include "imageanalyzer.h"
#include <cv.h>
#include <highgui.h>
#include<QList>
#include "simonvision_export.h"
#include<QThread>
#include <QMutex>
class SIMONVISION_EXPORT WebcamDispatcher : public QThread
{


public:
  WebcamDispatcher() {};

  ~WebcamDispatcher();

  static void registerAnalyzer(ImageAnalyzer* analyzer);
  static void reread();
  static void unregisterAnalyzer(ImageAnalyzer* analyzer);


private:

  // Webcam disptacher will be initialized when there is atleast on Analyzer registered
  void initWebcamDispatcher();

  // Webcam disptacher will be closed when there is no Analyzer registered
  void closeWebcamDispatcher();

  // This is the list of all the analyzers which are registered
  QList<ImageAnalyzer*> analyzers;

  // This is method implemented from QThread, Here we will be sending live feed to the analyzers
  void run();
  QMutex mutex;
  // Using Singleton pattern
  static WebcamDispatcher* instance;

};

#endif // WEBCAMDISPATCHER_H


