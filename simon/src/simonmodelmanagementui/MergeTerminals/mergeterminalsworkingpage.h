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

#ifndef SIMON_MERGETERMINALSWORKINGPAGE_H_8EDA9465BFB2449C8CD156F41B54419F
#define SIMON_MERGETERMINALSWORKINGPAGE_H_8EDA9465BFB2449C8CD156F41B54419F

#include <QWizardPage>
#include "ui_mergeterminalsworkingpage.h"
/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class MergeTerminals;
class MergeTerminalsWorkingPage : public QWizardPage
{
  Q_OBJECT
    signals:
  void done();
  private:
    Ui::MergeTerminalsWorkingPage ui;
    MergeTerminals *mergeTerminals;
    bool complete;
  private slots:
    void displayProgress(int progress, int max);
    void displayStatus(QString status);
    void finished();

  public:
    MergeTerminalsWorkingPage(QWidget* parent);
    bool isComplete() const { return this->complete; }
    void initializePage();
    ~MergeTerminalsWorkingPage();

};
#endif
