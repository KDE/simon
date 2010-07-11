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

#ifndef SIMON_IMPORTDICTWORKINGPAGE_H_3ED14C8D1F724EB09F099357310BB448
#define SIMON_IMPORTDICTWORKINGPAGE_H_3ED14C8D1F724EB09F099357310BB448

#include <QWizardPage>
#include <simonscenarios/word.h>

class QProgressBar;
class QLabel;
class ImportDict;
class KUrl;

/**
 \class ImportDictWorkingPage
 \brief This class actually imports the dict. by using the ImportDict class and displays the progress
 \author Peter Grasch
 \date 10.8.2007
 \version 0.1
*/
class ImportDictWorkingPage : public QWizardPage
{
  Q_OBJECT
    signals:
  void done();
  void failed();

  private:
    QProgressBar *pbMain;
    ImportDict *import;                           //!< Underlying concept class
    QLabel *lbStatus;
    bool ready;
    QString prepareDict(KUrl url);

  private slots:
    void setCompleted() { ready=true; emit completeChanged(); }
    QString guessEncoding(const QString& path);

  public slots:
    void abort();
    void displayStatus(QString status);
    void displayProgress(int progress, int max);

    void importHADIFIX(QString path);
    void importLexicon(QString path);
    void importPLS(QString path);
    void importSPHINX(QString path);
    void importJulius(QString path);

    bool isComplete() const;

  public:
    void initializePage();
    QList<Word*>* getCurrentWordList();
    ImportDictWorkingPage(QWidget* parent);

    ~ImportDictWorkingPage();

};
#endif
