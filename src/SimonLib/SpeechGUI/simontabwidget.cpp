#include "simontabwidget.h"
#include <QMessageBox>



SimonTabWidget::SimonTabWidget( QWidget * parent )
{
	
}


SimonTabWidget::~SimonTabWidget()
{
	
}

void SimonTabWidget::setTab(QString text)
{
	int j = 0;
	QString actualtabtext;
		while(j < this->count())
		{
			actualtabtext = this->tabText(j);
			if (text == actualtabtext)
			{
				this->setCurrentIndex(j);
				return;
			}
			j++;
		}
}
