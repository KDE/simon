/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_PRONUNCIATIONTRAINING_H_8518680DB8A44F5B9A7BE97EADECD623
#define SIMON_PRONUNCIATIONTRAINING_H_8518680DB8A44F5B9A7BE97EADECD623

#include <QWidget>
#include <QList>
#include <simonactions/greedyreceiver.h>
#include "ui_pronunciationtrainingdlg.h"

class Word;

class PronunciationTraining : public QWidget, public GreedyReceiver
{
Q_OBJECT

private:
	Ui::Form ui;
	QString m_terminal;
	int m_currentWordIndex;
	QList<Word*> m_wordsToTest;
	QList<float> m_scores;

	void displayCurrentWord();

private slots:
	void next();
	void prev();
	void quit();

protected:
	void closeEvent(QCloseEvent *);

public:
	bool greedyTriggerRawList(RecognitionResultList* results);
	void init();
    PronunciationTraining(const QString& terminal, QWidget* parent=0);

    ~PronunciationTraining();

};

#endif

