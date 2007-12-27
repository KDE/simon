

#ifndef SIMONGROUPBOX_H
#define SIMONGROUPBOX_H

#include <QGroupBox>

class SimonGroupBox : public QGroupBox
{	
	
	Q_OBJECT
	
private:
	
public:
	SimonGroupBox( QWidget * parent  = 0);
	~SimonGroupBox();

public slots:
	void changeCheckState();

	
};


#endif
