

#ifndef SIMONSLIDER_H
#define SIMONSLIDER_H

#include <QSlider>
#include <QLineEdit>
#include <QLabel>

class SimonSlider : public QSlider
{	
	
	Q_OBJECT
	
private:
	//QLabel lbPlusMinus;
	QLineEdit *lePlusMinus;
	
	
	
public:
	SimonSlider( QWidget * parent  = 0);
	~SimonSlider();

public slots:
	void changeValue();
	void doCommand();

	
};


#endif
