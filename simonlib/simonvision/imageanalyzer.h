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

#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H
#include <cv.h>
#include <highgui.h>
#include<QObject>
#include "simonvision_export.h"

class SIMONVISION_EXPORT ImageAnalyzer : public QObject
{

public:
  ImageAnalyzer(QObject* parent = 0);
  ~ImageAnalyzer();
  virtual void analyze(const IplImage* currentImage)=0;

};

#endif // IMAGEANALYZER_H

