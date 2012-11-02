/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_REESTIMATIONCONFIG_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_REESTIMATIONCONFIG_H_18BCB183592A48D3962815FFDEA7C214

#include <QString>

class ModelCompilerHTK;

class ReestimationConfig
{
  private:
    QString m_command;
    ModelCompilerHTK *m_manager;
  public:
    ReestimationConfig(const QString& command, ModelCompilerHTK *manager) : m_command(command), m_manager(manager)
    {}

    QString command() { return m_command; }
    ModelCompilerHTK* manager() { return m_manager; }
};

#endif

