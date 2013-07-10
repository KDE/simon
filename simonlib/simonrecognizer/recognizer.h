/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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


#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <simonrecognitionresult/recognitionresult.h>
#include "simonrecognizer_export.h"

class RecognitionConfiguration;

/*!
 *  \class Recognizer
 *  \brief The Recognizer class initialize recognition with given configuration
 *         and recognize audio files using method which specifed in inherited classes.
 *
 *  \version 0.1
 *  \date 14.08.2012
 *  \author Vladislav Sitalo
 */
class SIMONRECOGNIZER_EXPORT Recognizer
{
protected:
  QString m_lastError;

  QByteArray log;

public:
  virtual bool init(RecognitionConfiguration* config)=0;
  virtual QList<RecognitionResult> recognize(const QString& file)=0;

  //optional continuous recognition
  virtual bool startSample(const QString& /*file*/) { return false; }
  virtual bool feedSampleData(const QString& /*file*/, const QByteArray& /*data*/) { return false; }
  virtual QList<RecognitionResult> endSample(const QString& /*file*/) { return QList<RecognitionResult>(); }

  virtual bool uninitialize()=0;
  
  QString getLastError() { return m_lastError; }
  
  virtual QByteArray getLog() { return log; }
  
  virtual ~Recognizer() {}
};

#endif // RECOGNIZER_H
