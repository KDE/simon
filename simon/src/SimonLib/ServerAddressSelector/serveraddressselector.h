//
// C++ Interface: serveraddressselector
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SERVERADDRESSSELECTOR
#define SERVERADDRESSSELECTOR

#include <QWidget>

class KLineEdit;
class QToolButton;

class ServerAddressSelector : public QWidget
{
Q_OBJECT
private:
	KLineEdit *leServerAddress;
	QToolButton *pbSelectServerAddress;

private slots:
	void displayAddDialog();
public:
	ServerAddressSelector(QWidget *parent);

	KLineEdit* lineEdit();

	~ServerAddressSelector();

};
#endif
