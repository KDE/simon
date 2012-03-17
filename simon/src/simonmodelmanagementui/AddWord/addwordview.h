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

/**
 *	@class AddWordView
 *	@brief The wizard to add a new word to the Model
 *
 *	This wizard is using a step-by-step guide through the procedure
 *	to add a new word to the Language Model.
 *
 *	@version 0.1
 *	@date 08.01.2006
 *	@author Peter Grasch
 */

#ifndef SIMON_ADDWORDVIEW_H_617A2BD9B170401A8F0645BFFA7554B5
#define SIMON_ADDWORDVIEW_H_617A2BD9B170401A8F0645BFFA7554B5

#include <simonuicomponents/simonwizard.h>
#include <QStringList>
#include <QHash>
#include <simonscenarios/word.h>
#include "../simonmodelmanagementui_export.h"

class QWizardPage;
class AddWordRecordPage;
class AddWordResolvePage;
class AddWordIntroPage;
class Vocabulary;

class SIMONMODELMANAGEMENTUI_EXPORT AddWordView : public SimonWizard
{
  Q_OBJECT

    private:
    Vocabulary *targetVocabulary;

    QList<Word*> *listToAdd;
    QHash<QString,QString> promptsToAdd;
    QHash<QString,QString> sampleGroupsToAdd;

    void hideEvent(QHideEvent *event) {
      emit hidden(); return QWidget::hideEvent(event);
    }

    AddWordRecordPage *record1, *record2;

    void commitList();
    void advanceToResolvePage();

    signals:
    void addedWord();
    void hidden();

  private slots:
    void askToAddWord(QString word);
    void cleanUp();

  public slots:
    AddWordIntroPage* createWelcomePage();
    AddWordRecordPage* createRecordPage(const QString& fieldName, int pageNr, int pageMax);
    AddWordResolvePage* createResolvePage();
    QWizardPage* createFinishedPage();
    void addWords(QStringList words);

    void accept();

  public:
    explicit AddWordView(Vocabulary *vocab=0, QWidget *parent=0);
    ~AddWordView();

    void createWord(QString word);
};
#endif
