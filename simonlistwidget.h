

#ifndef SIMONLISTWIDGET_H
#define SIMONLISTWIDGET_H

#include <QLineEdit>
#include <QListWidget>

class SimonListWidget : public QListWidget
{	
	
	Q_OBJECT
	
private:
	
	QLineEdit *line;
	bool redFlag;
	
public:
	SimonListWidget( QWidget * parent  = 0);
	~SimonListWidget();
	
	void resizeEvent(QResizeEvent * event);
	void keyPressEvent ( QKeyEvent * event );
	void controlRedFlag();
	void showAllEntries();

public slots:
	void showLineEdit();
	void filterEntries(QString text);
	void moveLineEdit();
	void selectItem();
	
	
};


#endif
