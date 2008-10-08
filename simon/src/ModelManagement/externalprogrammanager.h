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


#ifndef EXTERNALPROGRAMMANAGER_H
#define EXTERNALPROGRAMMANAGER_H

#include "ui_externalprogramms.h"
#include <QWidget>

/**
 * \class ExternalProgramManager
 * \author Peter Grasch
 * \brief SystemWidgets that manages the paths to external programs
 * \date 13.08.2007
 * \version 0.1
 */
class ExternalProgramManager : public QWidget
{
Q_OBJECT
private:
	Ui::ExternalProgrammsDlg ui;

public:
    ExternalProgramManager(QWidget* parent);
    ~ExternalProgramManager();

};

#endif
