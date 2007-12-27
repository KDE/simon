//
// C++ Implementation: WindowsEvents
//
// Description: 
//
//
// Author: Phillip Goriup , (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logger.h"
#include <QCoreApplication>
#include "windowsevents.h"
#include <QMessageBox>
#include <ctype.h>


/**
 * @brief Constructor
 * 
 * Opens the display (member)
 *
 * @param char* displayName
 * The name of the display to open
 * 
 * @author Phillip Goriup
 */
WindowsEvents::WindowsEvents()
{
	this->keycodes = new QHash <char, int>();
	keycodes->insert('0',0x30);
	keycodes->insert('1',0x31);
	keycodes->insert('2',0x32);
	keycodes->insert('3',0x33);
	keycodes->insert('4',0x34);
	keycodes->insert('5',0x35);
	keycodes->insert('6',0x36);
	keycodes->insert('7',0x37);
	keycodes->insert('8',0x38);
	keycodes->insert('9',0x39);
	keycodes->insert('a',0x41);
	keycodes->insert('b',0x42);
	keycodes->insert('c',0x43);
	keycodes->insert('d',0x44);
	keycodes->insert('e',0x45);
	keycodes->insert('f',0x46);
	keycodes->insert('g',0x47);
	keycodes->insert('h',0x48);
	keycodes->insert('i',0x49);
	keycodes->insert('j',0x4A);
	keycodes->insert('k',0x4B);
	keycodes->insert('l',0x4C);
	keycodes->insert('m',0x4D);
	keycodes->insert('n',0x4E);
	keycodes->insert('o',0x4F),
	keycodes->insert('p',0x50);
	keycodes->insert('q',0x51);
	keycodes->insert('r',0x52);
	keycodes->insert('s',0x53);
	keycodes->insert('t',0x54);
	keycodes->insert('u',0x55);
	keycodes->insert('v',0x56);
	keycodes->insert('w',0x57);
	keycodes->insert('x',0x58);
	keycodes->insert('y',0x59);
	keycodes->insert('z',0x5A);
	
	keycodes->insert(' ',0x20);
	keycodes->insert('+',0xBB);
	keycodes->insert(',',0xBC);
	keycodes->insert('-',0xBD);
	keycodes->insert('.',0xBE);
	keycodes->insert('#',0xBF);
	keycodes->insert('ü',0xBA);
	keycodes->insert('ö',0xC0);
	keycodes->insert('ä',0xDE);
	keycodes->insert('´',0xDD);
	keycodes->insert('^',0xDC);
	keycodes->insert('ß',0xDB);
	keycodes->insert('<',0xE2);
	keycodes->insert('\n',0x0D);
	keycodes->insert('\r',0x0D);
	keycodes->insert('\r\n',0x0D);
	keycodes->insert('\t',0x09);


	this->shiftcodes = new QHash <char, char>();
	shiftcodes->insert('!','1');
	shiftcodes->insert('\"','2');
	shiftcodes->insert('§','3');
	shiftcodes->insert('$','4');
	shiftcodes->insert('%','5');
	shiftcodes->insert('&','6');
	shiftcodes->insert('/','7');
	shiftcodes->insert('(','8');
	shiftcodes->insert(')','9');
	shiftcodes->insert('=','0');
	shiftcodes->insert('?','ß');
	shiftcodes->insert('?','/');
	shiftcodes->insert('\'','#');
	shiftcodes->insert(';',',');
	shiftcodes->insert(':','.');
	shiftcodes->insert('_','-');
	shiftcodes->insert('*','+');
	shiftcodes->insert('Ü','ü');
	shiftcodes->insert('Ö','ö');
	shiftcodes->insert('Ä','ä');
	shiftcodes->insert('`','´');
	shiftcodes->insert('°','^');
	shiftcodes->insert('>','<');
	
	
	
	
	this->altgrcodes = new QHash <char, char>();
	altgrcodes->insert('²','2');
	altgrcodes->insert('³','3');
	altgrcodes->insert('{','7');
	altgrcodes->insert('[','8');
	altgrcodes->insert(']','9');
	altgrcodes->insert('}','0');
	altgrcodes->insert('\\','ß');
	altgrcodes->insert('@','q');
	altgrcodes->insert('€','e');
	altgrcodes->insert('~','+');
	altgrcodes->insert('|','<');
	altgrcodes->insert('µ','m');
}


void WindowsEvents::sendKey(int key)
{
	Sleep(100);
	keybd_event(key,0,0,0);	
}



void WindowsEvents::click(int x, int y)
{
	//TODO Implement me!
}

void WindowsEvents::sendKey(unsigned short key /*unicode representation*/)
{

	//TODO Implement me!
}

void WindowsEvents::sendShortcut(Shortcut shortcut)
{
	//TODO Implement me!
}

void WindowsEvents::sendChar(char key)
{	
	//QMessageBox::critical(NULL,"","");
	if (shiftcodes->contains(key))
	{ 	
		key = shiftcodes->value(key);
		setModifierKey(VK_LSHIFT,false);
	}
	if (altgrcodes->contains(key))
	{
		key = altgrcodes->value(key);
		setModifierKey(VK_RMENU,false);
	}
	this->sendKey(this->keycodes->value(key));
	unsetModifier(VK_LSHIFT);
	unsetModifier(VK_RMENU);
}


/**
 * @brief
 *
 * @param int virtualKey

 * @author Phillip Goriup
 */
void WindowsEvents::setModifierKey(int virtualKey, bool once)
{
	keybd_event(virtualKey,0,0,0);
}

/**
 * @brief 
 *
 * @param int virtualKey
 * 
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::unsetModifier(int virtualKey)
{
	keybd_event(virtualKey,0,KEYEVENTF_KEYUP,0);
}

/**
 * @brief Destructor
 *
 * 
 * 
 * @author Phillip Goriup
 */
WindowsEvents::~WindowsEvents()
{
}


