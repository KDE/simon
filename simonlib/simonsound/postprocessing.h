/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_POSTPROCESSING_H_8C5AEA136B514546BA2DFAE2AEB25B54
#define SIMON_POSTPROCESSING_H_8C5AEA136B514546BA2DFAE2AEB25B54

#include "simonsound_export.h"
#include <QObject>

class QString;

/**
 \class PostProcessing
 \author Peter Grasch
 \version 0.1
 \date 19.2.2008
 \brief Applies the specified postprocessing stack to the given filenames
*/
class SIMONSOUND_EXPORT PostProcessing : public QObject
{
  Q_OBJECT
    signals:
  void error(const QString& error);

  public:
    PostProcessing(QObject *parent=0);

    bool process(const QString& in, const QString& out, bool deleteIn=false, bool silent=false);

    ~PostProcessing();

};
#endif
