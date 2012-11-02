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

#ifndef SIMON_MERGETERMINALSSELECTTERMINALSPAGE_H_9DDC9A5462034D2989F1B6EE6A811D47
#define SIMON_MERGETERMINALSSELECTTERMINALSPAGE_H_9DDC9A5462034D2989F1B6EE6A811D47

#include <QWizardPage>

#include "ui_mergeterminalsselectpage.h"
/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class MergeTerminalsSelectTerminalsPage : public QWizardPage
{
  Q_OBJECT
    private:
    Ui::MergeTerminalsSelectTerminalsPage ui;
  public:
    MergeTerminalsSelectTerminalsPage(QWidget* parent);

    void initializePage();

    ~MergeTerminalsSelectTerminalsPage();

};
#endif
