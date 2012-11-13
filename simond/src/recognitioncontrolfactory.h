/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_RECOGNITIONCONTROLFACTORY_H
#define SIMON_RECOGNITIONCONTROLFACTORY_H

#include <QMultiHash>
#include <QHash>
#include <QString>
#include "recognitioncontrol.h"

class ModelIdentifier {
public:
  QString userName;
  RecognitionControl::BackendType backend;
  ModelIdentifier(const QString& u, RecognitionControl::BackendType b) : userName(u), backend(b) {}
  bool operator==(const ModelIdentifier& other) const {
    return (userName == other.userName) && (backend == other.backend);
  }
};

class RecognitionControlFactory
{
public:
  RecognitionControlFactory();
  RecognitionControl* recognitionControl(const QString& user, RecognitionControl::BackendType type);
  void closeRecognitionControl(RecognitionControl* r);
  void setIsolatedMode(bool isolatedMode);
  
private:
  QMultiHash<ModelIdentifier, RecognitionControl*> m_recognitionControls;
  bool m_isolatedMode;
};

uint qHash(const ModelIdentifier& identifier);

#endif // RECOGNITIONCONTROLFACTORY_H
