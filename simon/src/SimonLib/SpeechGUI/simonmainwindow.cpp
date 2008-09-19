#include "simonmainwindow.h"

SimonMainWindow::SimonMainWindow(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
	guievents = new GuiEvents;
}


SimonMainWindow::~SimonMainWindow()
{
	
}


void SimonMainWindow::registerControl(QString trigger, QObject* receiver, const char* slot)
{
	guievents->registerControl(trigger, receiver, slot);
}


void SimonMainWindow::doAction(QString action)
{
	guievents->doAction(action,(QObject*)this);
}


QObjectList SimonMainWindow::getChildren(QObject *current)
{
	return current->children();
}
