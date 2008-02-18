

#ifndef SIMONWIZAR_H
#define SIMONWIZAR_H

#include <QWizard>

/*typedef QHash<QObject*,  const char*> ActionIdent;*/
class SimonControl;

class SimonWizard : public QWizard
{
private:
	SimonControl *control;	
	/*QHash<QString,  ActionIdent> guiItems;*/
public:		
	SimonWizard(QWidget* parent=0);
	~SimonWizard();
	/*void registerControl(QString trigger, QObject* receiver, const char* slot);
	void doAction(QString action);*/



signals:
	void dummy();
	
	
	
};

#endif
