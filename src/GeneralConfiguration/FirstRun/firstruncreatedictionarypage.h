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
#include "../../ModelManagement/WordList/AddWord/addwordview.h"

/**
 \class FirstRunCreateDictionaryPage
 \author Peter Grasch
 \version 0.1
 \date 9.1.2008
 \brief Creates a WordListManager and a TrainingManager and publishes them
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
    FirstRunCreateDictionaryPage(QWidget* parent);
	void initializePage();

private:
    AddWordView* addWordView;

};

#endif
