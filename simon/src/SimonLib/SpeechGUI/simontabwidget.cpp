#include "simontabwidget.h"
#include <KMessageBox>



SimonTabWidget::SimonTabWidget( QWidget * parent ) : QTabWidget(parent)
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
