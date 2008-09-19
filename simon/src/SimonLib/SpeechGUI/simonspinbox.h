

#ifndef SIMONSPINBOX_H
#define SIMONSPINBOX_H

#include <KIntNumInput>

class SimonSpinBox : public KIntNumInput
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonSpinBox( QWidget * parent  = 0);
	~SimonSpinBox();

public slots:
	void changeValue(QString text);

	
};


#endif
