//
// C++ Interface: shortcut
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QList>
#include <QString>
#include "key.h"


class Shortcut;

typedef QList<Shortcut*> ShortcutList;

/**
 * \class Shortcut
 * \brief Small class that contains all the data to make up a shortcut
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
*/



class Shortcut {

private:
	static QList<Key*> keys;
	int modifiers /*Ctrl, Alt,...*/,
	    actionKeys /*Esc, Backspace, Return,...*/,
	    functionKeys /*F1,F2,F3,...*/,
	    movementKeys /*arrow-keys, home, end,...*/;
	
	char charKey;
public:
	Shortcut(int modifiers, int actionKeys, int functionKeys, int movementKeys, char charKey)
	{
		this->modifiers = modifiers;
		this->actionKeys = actionKeys;
		this->functionKeys = functionKeys;
		this->movementKeys = movementKeys;
		this->charKey = charKey;
	}


	const int getModifiers() const { return modifiers; }
	const int getActionKeys() const { return actionKeys; }
	const int getFunctionKeys() const { return functionKeys; }
	const int getMovementKeys() const { return movementKeys; }
	const char getCharKey() const { return charKey; }
	
	const QString getDescription() const;

	static void initKeys();
	static const int qtKeyToSimonKey ( int qtKey );
	static Key* getQtKey ( int qtKey );
	static const int simonKeyToqtKey ( int simonKey, short type );
	static const QString getSimonKeyname ( int simonKey, short type );
	
	bool operator==(Shortcut s2) {
		return ((modifiers==s2.getModifiers()) && 
			(s2.getActionKeys() == actionKeys) && (s2.getFunctionKeys() == functionKeys) &&
			(s2.getMovementKeys() == movementKeys) && (s2.getCharKey() == charKey));
	}

	~Shortcut() {}

};

#endif
