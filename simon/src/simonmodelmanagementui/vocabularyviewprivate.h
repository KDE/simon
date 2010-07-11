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

#ifndef SIMON_VOCABULARYVIEWPRIVATE_H_4A93354D9FCB466B95864B97C35249B2
#define SIMON_VOCABULARYVIEWPRIVATE_H_4A93354D9FCB466B95864B97C35249B2

#include "ui_vocabulary.h"

#include <simonscenarios/word.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/wordlisttype.h>
#include <simonscenarios/scenariodisplay.h>

class QSortFilterProxyModel;
class AddWordView;

/**
 *	@class VocabularyViewPrivate
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

class VocabularyViewPrivate : public QWidget, public ScenarioDisplay
{
  Q_OBJECT
    private:
    QSortFilterProxyModel *activeProxy;
    QSortFilterProxyModel *shadowProxy;

    Ui::VocabularyView ui;

    QList<Word*> trainingVocabulary;

    Word *getCurrentlySelectedWord(bool &isShadowed);

  public slots:
    void refreshActiveView();
    void refreshShadowView();
    void showImportDictDialog();
    void displayScenarioPrivate(Scenario *scenario);

    void editSelectedWord();
    void clear();
    void deleteSelectedWord();

    void copyWordToTrain();
    void deleteTrainingWord();
    void trainList();

  public:
    VocabularyViewPrivate(QWidget *parent);

    ~VocabularyViewPrivate();

};
#endif
