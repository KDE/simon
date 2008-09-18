#include "coreevents.h"
#include <QKeySequence>

CoreEvents::CoreEvents()
{
	strgSet=false;
	shiftSet=false;
	altgrSet=false;
	altSet=false;
	superSet=false;

	strgOnce=false;
	shiftOnce=false;
	altgrOnce=false;
	altOnce=false;
	superOnce=false;
}

void CoreEvents::unsetUnneededModifiers()
{
	if (shiftOnce) {
		shiftOnce=false;
		unsetModifier(Qt::SHIFT);
	}
	if (altgrOnce) {
		altgrOnce=false;
		unsetModifier(Qt::Key_AltGr);
	}
	if (strgOnce) {
		strgOnce=false;
		unsetModifier(Qt::CTRL);
	}
	if (altOnce) {
		altOnce=false;
		unsetModifier(Qt::ALT);
	}
	if (superOnce) {
		superOnce=false;
		unsetModifier(Qt::META);
	}
}




/**
 * \brief Sends the given Shortcut
 * 
 * This will set all the given modifiers and keys and then revert to the original
 * position (assuming the once flag is correctly interpreted)
 * 
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
void CoreEvents::sendShortcut(const QKeySequence& shortcut)
{
	int key = shortcut[0] & ~(Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);
	Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(shortcut[0] & (Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT));
	setModifierKey(mods, true);
	sendKey(key);
	unsetUnneededModifiers();
}
