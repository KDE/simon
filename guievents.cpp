#include "guievents.h"
#include <QRegExp>
#include <QMessageBox>
#include "logger.h"


GuiEvents::GuiEvents(QObject *parent)
{
	guiItems = new GuiAction;
}



GuiEvents::~GuiEvents()
{
	delete(guiItems);
}


void GuiEvents::registerControl(QString trigger, QObject* receiver, const char* slot)
{	
	
	actionValues *realAction = new actionValues;
	realAction->receiver = receiver;
	realAction->slot = slot;
	uniqueKey *key = new uniqueKey;

	key->trigger = trigger.trimmed();

	guiItems->insert(key, realAction);
}


void GuiEvents::doAction(QString action, QObject * parentItem)
{
	QRegExp expression("Kalender (Tag|Monat|Jahr) *");

		
	
	QList<uniqueKey*> keyList = guiItems->keys();
	int i = 0;
	int index = 0;
	uniqueKey *key;
	while(i < keyList.count()-1)
	{
		keyList.at(i)->trigger = keyList.at(i)->trigger.trimmed();

		if((keyList.at(i)->trigger) == action || expression.exactMatch((keyList.at(i)->trigger)))
		{	
			key = keyList.at(i);
			i = keyList.count()-1;
			index = 1;
		}	
		i++;
	}
	
	
	if (index == 0)
	{
		return;
	}
	connect(this, SIGNAL(dummy(QString)), guiItems->value(key)->receiver, guiItems->value(key)->slot);
	emit this->dummy(action);
	disconnect(this, SIGNAL(dummy(QString)),0,0);

}
