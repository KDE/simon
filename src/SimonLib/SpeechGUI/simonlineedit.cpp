#include "simonlineedit.h"




SimonLineEdit::SimonLineEdit( QWidget * parent ) : QLineEdit(parent)
{

}

SimonLineEdit::~SimonLineEdit()
{
	
}

void SimonLineEdit::focusOutEvent(QFocusEvent* event)
{
	emit checkFocus();
}

