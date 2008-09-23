#include "simonlineedit.h"




SimonLineEdit::SimonLineEdit( QWidget * parent ) : KLineEdit(parent)
{

}

SimonLineEdit::~SimonLineEdit()
{
	
}

void SimonLineEdit::focusOutEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	emit checkFocus();
}

