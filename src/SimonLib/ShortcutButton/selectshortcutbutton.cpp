//
// C++ Implementation: selectshortcutbutton
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "selectshortcutbutton.h"
#include <QKeyEvent>
#include "../EventSimulation/shortcut.h"

/**
 * \warning this class is fugly
 */

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the button
 */
SelectShortcutButton::SelectShortcutButton(QWidget *parent)
 : QPushButton(parent)
{
	reset();
	setCheckable(true);
	connect(this, SIGNAL(toggled(bool)), this, SLOT(grabKeys(bool)));
	keyPressCounter=0;
}


/**
 * \brief Starts the whole process of grabbing the keys and displaying it in the widget
 * \author Peter Grasch
 * @param start True if we want to start - else we want to stop
 */
void SelectShortcutButton::grabKeys(bool start)
{
	if (!start) {
		emit shortcutCreated(getShortcut());
		return;
	}

	if (start)
		grabKeyboard();
	
	modifierKeys_backup = modifierKeys;
	actionKeys_backup = actionKeys;
	functionKeys_backup = functionKeys;
	movementKeys_backup = movementKeys;
	reset();

	keyPressCounter=0;
}


/**
 * \brief Resets the Text and the buttons
 * \author Peter Grasch
 */
void SelectShortcutButton::reset()
{
	selectedKeys.clear();
	resetShortcutKeys();
	charKey='\0';
}

/**
 * \brief Resets the stored shortcut-keys
 * \author Peter Grasch
 */
void SelectShortcutButton::resetShortcutKeys()
{
	modifierKeys=0;
	actionKeys=0;
	functionKeys=0;
	movementKeys=0;
}

/**
 * \brief The key-press event
 * 
 * Here we will add every pressed key to the list of keys belonging to the shortcut
 * 
 * \author Peter Grasch
 * @param event The keyevent
 */
void SelectShortcutButton::keyPressEvent(QKeyEvent *event)
{
	event->accept();
	if (!isChecked()) return;

	keyPressCounter++;

	QString name = event->text();
	if (((event->key()>=0x30) && (event->key()<=0x0ff)) ||
		((!event->text().isEmpty()) && event->nativeModifiers() == 128 /*AltGr*/))
	{
		this->charKey = (char) event->key();
	} else 
		if (!selectedKeys.contains(event->key())) selectedKeys.append(event->key());

	displayKeys();
}

void SelectShortcutButton::setShortcut(const Shortcut *shortcut)
{
	modifierKeys = shortcut->getModifiers();
	actionKeys = shortcut->getActionKeys();
	functionKeys = shortcut->getFunctionKeys();
	movementKeys = shortcut->getMovementKeys();
	charKey = shortcut->getCharKey();
	displayKeys();
}

/**
 * \brief Builds a human-readable description of the pressed keys and displays it on the button
 * \author Peter Grasch
 */
void SelectShortcutButton::displayKeys()
{
	QString text;
	resetShortcutKeys();
	
	for (int i=0; i < selectedKeys.count(); i++)
	{
		QString newkey = "";

		Key *key = Shortcut::getQtKey(selectedKeys[i]);
		if (key)
		{
			newkey = key->getName();
			short type = key->getType();

			
			switch (type)
			{
				case ModifierKey:
					this->modifierKeys = modifierKeys|key->getSimonKey();
					break;
				case ActionKey:
					this->actionKeys = actionKeys|key->getSimonKey();
					break;
				case FunctionKey:
					this->functionKeys = functionKeys|key->getSimonKey();
					break;
				case MovementKey:
					this->movementKeys = movementKeys|key->getSimonKey();
					break;
			}
		}

		
		if (!newkey.isEmpty())
		{
			if (!text.isEmpty())
			{
				text = tr("%1 + %2").arg(text).arg(newkey);
			} else text = newkey;
		}
	}

	if (charKey != '\0')
	{
		if (!text.isEmpty())
			text += "+ "+QString(QChar(charKey));
		else text = charKey;
	}
	setText(text);
}

/**
 * \brief Returns the freshly build shortcut
 * 
 * This function will try to determine if the user actually entered a sequence and return the originally
 * stored shortcut if so
 * 
 * \author Peter Grasch
 * @return The shortcut
 */
Shortcut* SelectShortcutButton::getShortcut()
{
	if ((this->modifierKeys==0) && (this->actionKeys == 0) && (this->functionKeys==0) && (this->movementKeys==0) && (this->charKey=='\0'))
		return new Shortcut(this->modifierKeys_backup, this->actionKeys_backup, this->functionKeys_backup, this->movementKeys_backup, this->charKey_backup);
	else return new Shortcut(this->modifierKeys, this->actionKeys, this->functionKeys, this->movementKeys, this->charKey);
}

/**
 * \brief A key has been released
 * \author Peter Grasch
 * 
 * This function will check if this is the last key (all others have been released - determined by the
 * keyPressCounter (member)) and release the Keyboard if so;
 * It will also emit the shortcutCreated(...) signal if appropriate;
 * 
 * @param event 
 */
void SelectShortcutButton::keyReleaseEvent(QKeyEvent *event)
{
	if ((--keyPressCounter) == 0)
	{
		releaseKeyboard();
		setChecked(false);
		emit shortcutCreated(getShortcut());
	}
	event->accept();
}
