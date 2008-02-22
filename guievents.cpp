#include "guievents.h"
#include <QRegExp>
#include <QMessageBox>
#include "logger.h"
#include <QCoreApplication>
#include <ctype.h>

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


#include <qDebug>
void GuiEvents::doAction(QString action, QObject * parentItem)
{
	QRegExp expression("Kalender *");
	expression.setPatternSyntax(QRegExp::Wildcard);
	
	QList<uniqueKey*> keyList = guiItems->keys();
	int i = 0;
	int index = 0;
	uniqueKey *key;
	while(i < keyList.count()-1)
	{
		keyList.at(i)->trigger = keyList.at(i)->trigger.trimmed();

		if((keyList.at(i)->trigger) == action)
		{	
			key = keyList.at(i);
			i = keyList.count()-1;
			index = 1;
		}	
		//this else contains the code to aviable the controling of SimonCalendarWidget
		else if ((expression.exactMatch(action)) && (expression.exactMatch((keyList.at(i)->trigger))))
		{
			QStringList list = action.split(" ");
			
			int count = list.at(list.count()-1).count();
			
			qDebug() << count << " ; " << list.at(list.count()-1);
			
			QString temp = action;  
			temp = temp.remove(action.count()-count,count);
			temp = temp.trimmed();
			qDebug() << action.count()-count << " : " << temp << " == " << keyList.at(i)->trigger;
			if(temp == keyList.at(i)->trigger)
			{
				key = keyList.at(i);
				i = keyList.count()-1;
				index = 1;
			}
		}
		i++;
	}
	
	
	if (index == 0)
	{
		return;
	}
	QCoreApplication::processEvents();
	//connects to the slot that is saved in the QHash with the given parameters
	connect(this, SIGNAL(dummy(QString)), guiItems->value(key)->receiver, guiItems->value(key)->slot);
	emit this->dummy(action);
	disconnect(this, SIGNAL(dummy(QString)),0,0);

}
