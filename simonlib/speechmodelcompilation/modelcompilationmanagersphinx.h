/*
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

#ifndef MODELCOMPILATIONMANAGERSPHINX_H
#define MODELCOMPILATIONMANAGERSPHINX_H

#include "modelcompilationmanager.h"
#include "simonmodelcompilationmanagement_export.h"

//#define GRAMMAR_EXT ".jsjf"
//#define PHONE_EXT ".phone"
//#define DICT_EXT ".dic"
//#define TRAIN_TRANSCRIPTION "_train.transcription"
//#define TRAIN_FIELDS "_train.fileids"

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManagerSPHINX: public ModelCompilationManager
{
  Q_OBJECT
public:
  ModelCompilationManagerSPHINX(const QString& userName, QObject *parent);
protected:
  //TODO: Implement
  void run();

};

#endif // MODELCOMPILATIONMANAGERSPHINX_H
