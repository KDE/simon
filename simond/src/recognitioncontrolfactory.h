/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#ifndef RECOGNITIONCONTROLFACTORY_H
#define RECOGNITIONCONTROLFACTORY_H

#include <QMultiHash>
#include <QString>
class RecognitionControl;

class RecognitionControlFactory
{
private:
  QMultiHash<QString, RecognitionControl*> m_recognitionControls;
  bool m_isolatedMode;

public: // TODO: friend for SimondControl
  void setIsolatedMode(bool isolatedMode);
  
public:
  RecognitionControlFactory();
  RecognitionControl* recognitionControl(const QString& user);
  void closeRecognitionControl(const QString& user, RecognitionControl* r);
  
};

#endif // RECOGNITIONCONTROLFACTORY_H
