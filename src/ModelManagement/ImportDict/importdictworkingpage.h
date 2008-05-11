//
// C++ Interface: importdictworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTWORKINGPAGE_H
#define IMPORTDICTWORKINGPAGE_H

#include <QWizardPage>
#include "word.h"

class QProgressBar;
class QLabel;
class ImportDict;

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
private slots:
	void importWiktionaryFile(QString path);
	void setCompleted() { ready=true; emit completeChanged(); }
public slots:
	void abort();
	void displayStatus(QString status);
	void displayProgress(int progress);
	
	void importHADIFIX(QString path);
	void importWiktionary(QString url);
	void importLexicon(QString path);

	void unpackWikiIfNecessary(QString file);

	bool isComplete() const;
public:
	void initializePage();
    ImportDictWorkingPage(QWidget* parent);

    ~ImportDictWorkingPage();

};

#endif
