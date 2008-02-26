#include "simonwizard.h"

SimonWizard::SimonWizard(QWidget* parent): QWizard(parent)
{
	//this->control = new SimonControl();
	//connect(control, SIGNAL(guiAction(QString)), this, SLOT(doAction(QString)));
	
	
	/*registerControl("Wort hinzufügen", ui.pbAddWord, SLOT(animateClick()));
	registerControl("Trainieren", ui.pbTrain, SLOT(animateClick()));
	registerControl("Wortliste", ui.pbEditWordList, SLOT(animateClick()));
	registerControl("Ausführen", ui.pbRunProgram, SLOT(animateClick()));
	registerControl("System", ui.pbSettings, SLOT(animateClick()));*/
	
}

SimonWizard::~SimonWizard()
{
	delete control;
}
/*
void SimonWizard::registerControl(QString trigger, QObject* receiver, const char* slot)
{
	ActionIdent action;
	action.insert(receiver,slot);
	guiItems.insert(trigger, action);
}


void SimonWizard::doAction(QString action)
{
	if (!guiItems.contains(action)) return;
	
	ActionIdent realAction = guiItems.value(action);
	connect(this, SIGNAL(dummy()), realAction.keys()[0], realAction.values()[0]);
	emit dummy();
	disconnect(this, SIGNAL(dummy()),0,0);
}
*/
