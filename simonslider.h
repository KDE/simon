

#ifndef SIMONSLIDER_H
#define SIMONSLIDER_H

#include <QSlider>
class QLineEdit;

class SimonSlider : public QSlider
{	
	
	Q_OBJECT
	
private:
	QLineEdit *lePlusMinus;
	
	
	
public:
	SimonSlider( QWidget * parent  = 0);
	~SimonSlider();

public slots:
	void changeValue();
	void doCommand();

	
};


#endif
