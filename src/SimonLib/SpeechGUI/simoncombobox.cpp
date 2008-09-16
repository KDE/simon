#include "simoncombobox.h"
#include <QMessageBox>




SimonComboBox::SimonComboBox( QWidget * parent ) : KComboBox(parent)
{
}

SimonComboBox::~SimonComboBox()
{
	
}

void SimonComboBox::schowPopUp()
{
	this->showPopup ();
}
