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

#ifndef SIMON_RENAMETERMINALWIZARD_H_81323ACF45B7425ABFB0297D096887DA
#define SIMON_RENAMETERMINALWIZARD_H_81323ACF45B7425ABFB0297D096887DA

#include <simonuicomponents/simonwizard.h>

/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class QWizardPage;

class RenameTerminalWizard : public SimonWizard
{
  Q_OBJECT
    private:
    QWizardPage *createIntroPage();
    QWizardPage *createSelectParametersPage();
    QWizardPage *createWorkingPage();
    QWizardPage *createFinishedPage();
  public:
    RenameTerminalWizard(QWidget* parent);

    ~RenameTerminalWizard();

};
#endif
