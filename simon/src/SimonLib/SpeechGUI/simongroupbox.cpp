#include "simongroupbox.h"

SimonGroupBox::SimonGroupBox( QWidget * parent ) : QGroupBox(parent)
{
	
}


SimonGroupBox::~SimonGroupBox()
{

}


void SimonGroupBox::changeCheckState()
{
	if (this->isCheckable())
	{
		if(this->isChecked())
			this->setChecked(false);
		else
			this->setChecked(true);
	}
}
