#include "simoncombobox.h"
#include <QMessageBox>




SimonComboBox::SimonComboBox( QWidget * parent ) : QComboBox(parent)
{
}

SimonComboBox::~SimonComboBox()
{
	
}

void SimonComboBox::schowPopUp()
{
	this->showPopup ();
}
