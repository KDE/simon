#include "simonspinbox.h"




SimonSpinBox::SimonSpinBox( QWidget * parent ) : KIntNumInput(parent)
{

}

SimonSpinBox::~SimonSpinBox()
{
	
}

void SimonSpinBox::changeValue(QString text)
{
	if (text.contains("up"))
		this->setValue(this->value()+10);
	else
		this->setValue(this->value()-10); 
}
