//
// C++ Interface: firstruncreatedictionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNCREATEDICTIONARYPAGE_H
#define FIRSTRUNCREATEDICTIONARYPAGE_H

#include <QWizardPage>
#include "addwordview.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WordListManager;
class TrainingManager;
class FirstRunCreateDictionaryPage : public QWizardPage
{
Q_OBJECT
signals:
	void wordListManagerCreated(WordListManager*);
	void trainingManagerCreated(TrainingManager*);
	void done();
public:
    FirstRunCreateDictionaryPage(AddWordView* addWordView, QWidget* parent);
	void initializePage();
    ~FirstRunCreateDictionaryPage();

private:
    AddWordView* addWordView;

};

#endif
