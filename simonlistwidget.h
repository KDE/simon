

#ifndef SIMONLISTWIDGET_H
#define SIMONLISTWIDGET_H

#include <QLineEdit>
#include <QListWidget>

class SimonListWidget : public QListWidget
{	
	
	Q_OBJECT

	/**
	 * \brief Define the current text as a property for use in wizards
	 * \author Peter Grasch
	*/
	Q_PROPERTY(QString currentText READ currentText)
	Q_PROPERTY(QVariant currentUserData READ currentUserData)

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

	/**
	 * \brief Quick access function to retrieve the currently selected text
	 * \author Peter Grasch
	 * @return The currently selected text (or a new QString object) if there is none selected
	 */
	QString currentText() { 
		QListWidgetItem *selected = currentItem();
		if (!selected) return QString();
		return currentItem()->text();
	}

public slots:
	QVariant currentUserData()
	{
		if (!currentItem()) return QVariant();
		return currentItem()->data(Qt::UserRole);
	}
	void showLineEdit();
	void filterEntries(QString text);
	void moveLineEdit();
	void selectItem();
	
	
};


#endif
