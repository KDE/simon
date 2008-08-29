//
// C++ Implementation: shortcut
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shortcut.h"
#include <QCoreApplication>

//static

QList<Key*> Shortcut::keys;

/**
 * \brief Returns a text like "Strg + A" which describes the shortcut
 * \author Peter Grasch
 * @return description
 */
QString Shortcut::getDescription() const
{
	QString modifierName = Shortcut::getSimonKeyname(getModifiers(), ModifierKey);
	QString actionName = Shortcut::getSimonKeyname(getActionKeys(), ActionKey);
	QString movementName = Shortcut::getSimonKeyname(getMovementKeys(), MovementKey);
	QString functionName = Shortcut::getSimonKeyname(getFunctionKeys(),FunctionKey);

	QString text="";
	text += modifierName;
	if ((!actionName.isEmpty()) && !text.isEmpty()) 
		text += " + ";
	text += actionName;

	if ((!movementName.isEmpty()) && !text.isEmpty()) 
		text += " + ";
	text += movementName;
	
	if ((!functionName.isEmpty()) && !text.isEmpty()) 
		text += " + ";
	text += functionName;
	
	if (getCharKey()!= '\0')
	{
		if (!text.isEmpty()) 
			text += " + ";
		text += QChar(getCharKey());
	}
	return text;
}




/**
 * \brief Converts a QT-Keycode to a simon-keycode
 * \author Peter Grasch
 * @param qtKey Qt-KeyCode (Qt::Key_*)
 * @return The matching simon-Keycode (or 0 if not found)
 */
int Shortcut::qtKeyToSimonKey ( int qtKey )
{
	for (int i=0; i < keys.count(); i++)
	{
		if (keys.at(i)->getQtKey() == qtKey)
			return keys.at(i)->getSimonKey();
	}
	return 0;
}


/**
 * \brief Returns the key referenced by the given qt-Keycode
 * \author Peter Grasch
 * @param qtKey 
 * @return The found Key (NULL if we found none)
 */
Key* Shortcut::getQtKey ( int qtKey )
{
	for (int i=0; i < keys.count(); i++)
	{
		if (keys.at(i)->getQtKey() == qtKey)
			return keys.at(i);
	}
	return NULL;
}

/**
 * \brief Returns a human-readable name of the given simonKey-code
 * \author Peter Grasch
 * @param simonKey The key to lookup
 * @param type Type of the given key (see the enum defined in shortcut.h)
 * @return The name of the key ("" if not found)
 */
QString Shortcut::getSimonKeyname ( int simonKey, short type )
{
	QString keyName="";
	for (int i=0; i < keys.count(); i++)
	{
		if ((keys.at(i)->getSimonKey() & simonKey) && (keys.at(i)->getType() == type))
		{
			if (!keyName.isEmpty()) keyName += " + ";
			keyName += keys.at(i)->getName();
		}
	}
	return keyName;
}



/**
 * \brief Returns the qt-Key of the given simonKey
 * \author Peter Grasch
 * @param simonKey The simon-keycode
 * @param type The type of the simon-key (see the enum in shortcut.h)
 * @return The corresponding qt keycode
 */
int Shortcut::simonKeyToqtKey ( int simonKey, short type )
{
	for (int i=0; i < keys.count(); i++)
	{
		if ((keys.at(i)->getSimonKey() == simonKey) && (keys.at(i)->getType() == type))
			return keys.at(i)->getQtKey();
	}
	return 0;
}

/**
 *  \brief Inits the Keys with the given entries
 *  \author Peter Grasch
 */
void Shortcut::initKeys()
{
	keys << new Key(KeyEnter, Qt::Key_Return, ActionKey, QCoreApplication::tr("Enter"));
	keys << new Key(KeyEscape, Qt::Key_Escape, ActionKey, QCoreApplication::tr("Esc"));
	keys << new Key(KeyPause, Qt::Key_Pause, ActionKey, QCoreApplication::tr("Pause"));
	keys << new Key(KeyPrintScr, Qt::Key_Print, ActionKey, QCoreApplication::tr("Druck"));
	keys << new Key(KeyBackspace, Qt::Key_Backspace, ActionKey, QCoreApplication::tr("Löschen"));

	keys << new Key(KeyArrowLeft, Qt::Key_Left, MovementKey, QCoreApplication::tr("<-"));
	keys << new Key(KeyArrowRight, Qt::Key_Right, MovementKey, QCoreApplication::tr("->"));
	keys << new Key(KeyArrowUp, Qt::Key_Up, MovementKey, QCoreApplication::tr("/\\"));
	keys << new Key(KeyArrowDown, Qt::Key_Down, MovementKey, QCoreApplication::tr("\\/"));
	keys << new Key(KeyEnd, Qt::Key_End, MovementKey, QCoreApplication::tr("Ende"));
	keys << new Key(KeyBegin, Qt::Key_Home, MovementKey, QCoreApplication::tr("Pos1"));
	keys << new Key(KeyPageUp, Qt::Key_PageUp, MovementKey, QCoreApplication::tr("Bild /\\"));
	keys << new Key(KeyPageDown, Qt::Key_PageDown, MovementKey, QCoreApplication::tr("Bild \\/"));

	keys << new Key(KeyF1, Qt::Key_F1, FunctionKey, QCoreApplication::tr("F1"));
	keys << new Key(KeyF2, Qt::Key_F2, FunctionKey, QCoreApplication::tr("F2"));
	keys << new Key(KeyF3, Qt::Key_F3, FunctionKey, QCoreApplication::tr("F3"));
	keys << new Key(KeyF4, Qt::Key_F4, FunctionKey, QCoreApplication::tr("F4"));
	keys << new Key(KeyF5, Qt::Key_F5, FunctionKey, QCoreApplication::tr("F5"));
	keys << new Key(KeyF6, Qt::Key_F6, FunctionKey, QCoreApplication::tr("F6"));
	keys << new Key(KeyF7, Qt::Key_F7, FunctionKey, QCoreApplication::tr("F7"));
	keys << new Key(KeyF8, Qt::Key_F8, FunctionKey, QCoreApplication::tr("F8"));
	keys << new Key(KeyF9, Qt::Key_F9, FunctionKey, QCoreApplication::tr("F9"));
	keys << new Key(KeyF10, Qt::Key_F10, FunctionKey, QCoreApplication::tr("F10"));
	keys << new Key(KeyF11, Qt::Key_F11, FunctionKey, QCoreApplication::tr("F11"));
	keys << new Key(KeyF12, Qt::Key_F12, FunctionKey, QCoreApplication::tr("F12"));
	
	
	keys << new Key(KeyShift, Qt::Key_Shift, ModifierKey, QCoreApplication::tr("Shift"));
	keys << new Key(KeyStrg, Qt::Key_Control, ModifierKey, QCoreApplication::tr("Strg"));
	keys << new Key(KeyAlt, Qt::Key_Alt, ModifierKey, QCoreApplication::tr("Alt"));
	keys << new Key(KeyAltGr, Qt::Key_AltGr, ModifierKey, QCoreApplication::tr("(AltGr)"));
	keys << new Key(KeySuper, Qt::Key_Super_L, ModifierKey, QCoreApplication::tr("Super"));
}

