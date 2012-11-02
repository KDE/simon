/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SPHINXDICT_H_9A7CF3AD25134358BB7C3F31DAB5BF69
#define SIMON_SPHINXDICT_H_9A7CF3AD25134358BB7C3F31DAB5BF69

#include "dict.h"

class QString;

/**
 \class SPHINXDict
 \author Peter Grasch
 \version 0.1
 \date 10.08.2007
 \brief Represetnts a SPHINX dictionary
 */
class SPHINXDict : public Dict
{
  Q_OBJECT

    signals:
  void loaded();
  private:
    QString path;
  public:
    explicit SPHINXDict(QObject* parent=0);
    void load(QString path, QString encodingName);

    ~SPHINXDict();

};
#endif
