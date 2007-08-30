//
// C++ Interface: key
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEY_H
#define KEY_H



//modifiers
const int KeyShift=1;
const int KeyAlt=2;
const int KeyStrg=4;
const int KeySuper=8;
const int KeyCapsLock=16;
const int KeyAltGr=32;

//action
const int KeyBackspace=1;
const int KeyEscape=2;
const int KeyClear=4;
const int KeyPrintScr=8;
const int KeyPause=16;
const int KeyUndo=32;
const int KeyRedo=64;
const int KeyEnter=128;

//f-tasten
const int KeyF1=1;
const int KeyF2=2;
const int KeyF3=4;
const int KeyF4=8;
const int KeyF5=16;
const int KeyF6=32;
const int KeyF7=64;
const int KeyF8=128;
const int KeyF9=256;
const int KeyF10=512;
const int KeyF11=1024;
const int KeyF12=2048;

//movement
const int KeyArrowLeft=1;
const int KeyArrowRight=2;
const int KeyArrowDown=4;
const int KeyArrowUp=8;
const int KeyPageUp=16;
const int KeyPageDown=32;
const int KeyEnd=64;
const int KeyBegin=128;

enum keytype {
	ModifierKey,
	ActionKey,
	FunctionKey,
	MovementKey
};

class Key{


private:
	int simonKey;
	int qtKey;
	short type;
	QString name;

public:
	Key(int simonKey, int qtKey, short type, QString name="")
	{
		this->simonKey = simonKey;
		this->qtKey = qtKey;
		this->type = type;
		this->name = name;
	}
	
	const short getType() const { return this->type;  }
	const int getSimonKey() const { return simonKey; }
	const int getQtKey() const { return qtKey; }
	const QString getName() const { return name; }
	
	
	~Key();

};

#endif
