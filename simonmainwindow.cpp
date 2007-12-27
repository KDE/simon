#include "simonmainwindow.h"
#include <QMessageBox>



SimonMainWindow::SimonMainWindow(QWidget *parent, Qt::WFlags flags)
{
	guievents = new GuiEvents;
}

SimonMainWindow::~SimonMainWindow()
{
	
}


void SimonMainWindow::registerControl(QString trigger, QObject* receiver, const char* slot)
{
	//QMessageBox::critical(NULL,"",trigger);
	guievents->registerControl(trigger, receiver, slot);
}
void SimonMainWindow::doAction(QString action)
{
	//QMessageBox::critical(NULL,"MainWINDOW",action);
	guievents->doAction(action,(QObject*)this);
}

QObjectList SimonMainWindow::getChildren(QObject *current)
{
	return current->children();
}
