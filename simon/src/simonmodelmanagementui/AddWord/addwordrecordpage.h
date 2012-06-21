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

#ifndef SIMON_ADDWORDRECORDPAGE_H_E54814732D4746A581807C9D3C61441E
#define SIMON_ADDWORDRECORDPAGE_H_E54814732D4746A581807C9D3C61441E

#include <QWizardPage>
#include <QStringList>
#include <simonscenarios/promptstable.h>

class TrainSamplePage;

/**
 \class AddWordRecordPage
 \author Peter Grasch
 \date 01.08.2007
 \brief Records two samples of the new word

 The WizardPage of the AddWordView-Wizard that manages the recordings;
 Uses two Recwidgets and the QT-Mechanism to ensure that you cannot
 continiue without both recordings done.
*/
class AddWordRecordPage : public QWizardPage
{
  Q_OBJECT

    signals:
  void recordingNamesGenerated(QString,QString);
  private:
    TrainSamplePage *page;
    int pageNr, pageMax;
    QString fieldName;

    /// Normally a sample containing speech data of a different prompt than
    /// The current prompt stored in the given fieldName will be deleted
    /// when the page is shown (to prevent stale recordings when we go back
    /// from the recording page to change the prompt after we already recorded
    /// it).
    /// However, when adding multiple words we have to keep the previously
    /// recorded sample.
    /// This is a flag to keep _any_ previously recorded sample - ONCE.
    /// Setting this flag will disable the flag for the next check (where
    /// it is then reset).
    /// Use keepSample() set the flag;
    /// \sa keepSample()
    bool stickSample;

  public:
    PromptsTable getPrompts() const;
    void keepSample() { stickSample = true; }
    void cleanUp();
    void cleanupPage();

    bool validatePage();

    void initializePage();
    AddWordRecordPage(const QString& fieldName, int pageNr, int pageMax, QWidget *parent=0);

    bool isComplete() const;

    ~AddWordRecordPage();

};
#endif
