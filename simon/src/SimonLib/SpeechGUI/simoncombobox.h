

#ifndef SIMONCOMBOBOX_H
#define SIMONCOMBOBOX_H

#include <KComboBox>

class SimonComboBox : public KComboBox
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonComboBox( QWidget * parent  = 0);
	~SimonComboBox();

public slots:
	void schowPopUp();

	
};


#endif
