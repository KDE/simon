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


#ifndef MODELCOMPILATIONMANAGERHTK_H
#define MODELCOMPILATIONMANAGERHTK_H

#include "modelcompilationmanager.h"
#include "simonmodelcompilationmanagement_export.h"

class ModelCompilationAdapterHTK;
class ModelCompilerHTK;

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManagerHTK : public ModelCompilationManager
{
  Q_OBJECT
public:
  ModelCompilationManagerHTK(const QString& userName, QObject *parent);

protected:
  void run();
  
private slots:
  void slotPhonemeUndefined(const QString& phoneme);
};

#endif // MODELCOMPILATIONMANAGERHTK_H
