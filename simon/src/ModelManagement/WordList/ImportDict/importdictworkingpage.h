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


#ifndef IMPORTDICTWORKINGPAGE_H
#define IMPORTDICTWORKINGPAGE_H

#include <QWizardPage>
#include "../word.h"

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
	void wordListImported(WordList* wlist);
	void failed();
private:
	QProgressBar *pbMain;
	ImportDict *import; //!< Underlying concept class
	QLabel *lbStatus;
	bool ready;
	QString prepareDict(KUrl url);

private slots:
	void setCompleted() { ready=true; emit completeChanged(); }

public slots:
	void abort();
	void displayStatus(QString status);
	void displayProgress(int progress);
	
	void importHADIFIX(QString path);
	void importWiktionary(QString url);
	void importLexicon(QString path);

	bool isComplete() const;

public:
	void initializePage();
    ImportDictWorkingPage(QWidget* parent);

    ~ImportDictWorkingPage();

};

#endif
