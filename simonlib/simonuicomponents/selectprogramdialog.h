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

#ifndef SIMON_SELECTPROGRAMDIALOG_H_5C48BD3D50AE452B9063AD517C4E8E94
#define SIMON_SELECTPROGRAMDIALOG_H_5C48BD3D50AE452B9063AD517C4E8E94

#include <KDialog>
#include "simonuicomponents_export.h"

namespace Ui
{
class SelectProgramDlg;
}

/**
 * \class SelectProgramDialog
 * \author Peter Grasch
 * \date 16.08.2007 (moved from the Executable plugin to simonuicomponents by Adam Nash on 16.9.2011)
 * \version 0.1
 * \brief The user must choose a category, to display the corresponding programs, where he also must choose one to continue.
 */

class SIMONUICOMPONENTS_EXPORT SelectProgramDialog : public KDialog
{
  Q_OBJECT

    private:
    Ui::SelectProgramDlg *ui;

    void initialize();

    void findCategories(QString relPath);

  private slots:
    void searchForPrograms();

  public:
    SelectProgramDialog(QWidget* parent);
    ~SelectProgramDialog();

    bool selectCommand();

    QString getExecPath();
    QString getName();
    QString getIcon();
    QString getDescription();
    QString getWorkingDirectory();

};
#endif
