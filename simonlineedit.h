

#ifndef SIMONLINEEDIT_H
#define SIMONLINEEDIT_H

#include <QLineEdit>

class SimonLineEdit : public QLineEdit
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonLineEdit( QWidget * parent  = 0);
	~SimonLineEdit();

	void focusOutEvent (QFocusEvent* event);

signals:
	void checkFocus();

};


#endif
