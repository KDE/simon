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

#ifndef SIMON_MERGECATEGORIESWIZARD_H_14ABC8E4DBD54DDE86D831B9195E9C3D
#define SIMON_MERGECATEGORIESWIZARD_H_14ABC8E4DBD54DDE86D831B9195E9C3D

#include <simonuicomponents/simonwizard.h>

/**
  @author Peter Grasch <peter.grasch@bedahr.org>
*/
class QWizardPage;

class MergeCategoriesWizard : public SimonWizard
{
    Q_OBJECT

  public:
    MergeCategoriesWizard(QWidget* parent);

  private:
    QWizardPage* createIntroPage();
    QWizardPage* createSelectCategoriesPage();
    QWizardPage* createWorkingPage();
    QWizardPage* createFinishedPage();

};
#endif
