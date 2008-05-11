

#ifndef SIMONCOMBOBOX_H
#define SIMONCOMBOBOX_H

#include <QComboBox>

class SimonComboBox : public QComboBox
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
