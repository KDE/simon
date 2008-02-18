//
// C++ Interface: selectshortcutbutton
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTSHORTCUTBUTTON_H
#define SELECTSHORTCUTBUTTON_H

#include <QPushButton>
#include <QList>


class Shortcut;
class QKeyEvent;

/**
 * \class SelectShortcutButton
 * \brief A pushbutton which (when toggled) uses the pressed key to compose a shortcut
 * 
 * This class displays a button which reacts and almost all buttons on the keyboard and displays them
 * like "Alt + A" until you release the last pressed key;
 * It will then emit a shortcutCreated(...) signal containing the newly created shortcut
 * 
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
*/
class SelectShortcutButton : public QPushButton
{
	Q_OBJECT

signals:
	void shortcutCreated(Shortcut*);
private:
	QList<int> selectedKeys;
	char charKey, charKey_backup;

	QString name;
	int keyPressCounter;

	int modifierKeys, actionKeys, functionKeys, movementKeys;
	int modifierKeys_backup, actionKeys_backup, functionKeys_backup, movementKeys_backup;

	void reset();
	void resetShortcutKeys();
	
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void displayKeys();
	Shortcut* getShortcut();

private slots:
	void grabKeys(bool start);
public:
    SelectShortcutButton(QString name, QString text="", QWidget *parent=0);


    ~SelectShortcutButton() {}


};

#endif
