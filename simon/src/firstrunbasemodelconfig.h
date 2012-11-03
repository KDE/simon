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

#ifndef SIMON_FIRSTRUNBASEMODELCONFIG_H_FA01A01DFF1E4DA098606C3E951E432D
#define SIMON_FIRSTRUNBASEMODELCONFIG_H_FA01A01DFF1E4DA098606C3E951E432D

#include <QWizardPage>
#include "ui_firstrunbasemodelconfig.h"

class BaseModelSettings;
class FirstRunBaseModelConfig : public QWizardPage
{
    Q_OBJECT

  public:
    explicit FirstRunBaseModelConfig(QWidget *parent = 0);

  private slots:
    void initializePage();
    bool validatePage();

  private:
    Ui::BaseModelConfigDlg ui;
    BaseModelSettings *baseModelSettings;
};
#endif
