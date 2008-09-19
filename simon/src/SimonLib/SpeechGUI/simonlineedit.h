

#ifndef SIMONLINEEDIT_H
#define SIMONLINEEDIT_H

#include <KLineEdit>

class SimonLineEdit : public KLineEdit
{
	Q_OBJECT
	
	
public:
	SimonLineEdit( QWidget * parent  = 0);
	~SimonLineEdit();

	void focusOutEvent (QFocusEvent* event);

signals:
	void checkFocus();

};


#endif
