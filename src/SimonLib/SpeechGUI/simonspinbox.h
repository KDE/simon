

#ifndef SIMONSPINBOX_H
#define SIMONSPINBOX_H

#include <QSpinBox>

class SimonSpinBox : public QSpinBox
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
