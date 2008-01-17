#include "simonlistwidget.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QScrollBar>
#include <QPalette>
#include <QCoreApplication>
#include <QKeyEvent>




SimonListWidget::SimonListWidget( QWidget * parent ) : QListWidget(parent)
{
	line = new QLineEdit(this);
	line->hide();
	this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
	this->redFlag = true;
	connect(line,SIGNAL(textChanged(QString)),this,SLOT(filterEntries(QString)));
	//connect(line,SIGNAL(returnPressed () ),this,SLOT(selectItem()));
}

SimonListWidget::~SimonListWidget()
{
	
}

void SimonListWidget::showLineEdit()
{
	moveLineEdit();
	this->line->clear();
	line->show();
	line->setFocus () ;
}

void SimonListWidget::moveLineEdit()
{
	//QMessageBox::information(this,"","");
	line->move(this->width() - line->width()-10,this->height() - line->height()-10);
	//QCoreApplication::processEvents();
	//line->move(this->width() - line->width()-10,this->height() - line->height()-10);
}

void SimonListWidget::resizeEvent ( QResizeEvent * event )
{
	this->moveLineEdit();
}

void SimonListWidget::selectItem()
{
	int i;
	
	if((this->redFlag )|| (this->line->text() == ""))
		return;

	for (i = 0; i<this->count(); i++)
	{
		if (!this->item(i)->isHidden())
		{	
			this->setCurrentItem(this->item(i));
			scrollToItem (this->item(i), QAbstractItemView::PositionAtCenter);
			showAllEntries();
			this->line->hide();
			this->setFocus();
			return;
		}
	}
}

void SimonListWidget::filterEntries(QString text)
{
	int i = 0;
	this->redFlag = true;
	
	//setUpdatesEnabled (false) ;
	if (text == "")
	{
		this->redFlag = false;	
		//this->setCurrentItem(this->item(0));
		controlRedFlag();
		showAllEntries();
		return;
	}
	for (i = 0; i<this->count(); i++)
	{
		if(this->item(i)->text().startsWith(text, Qt::CaseInsensitive))
		{
			//this->setCurrentItem(this->item(i)+1);
			this->item(i)->setHidden(false);
			//return;
			this->redFlag = false;
		}
		else
		{
			this->item(i)->setHidden (true); 
		}
	}
	controlRedFlag();
	//setUpdatesEnabled (true);
}

void SimonListWidget::controlRedFlag()
{
	if (this->redFlag)
	{
		QColor color(224,67,67);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->line->palette();
		palette.setBrush(QPalette::Base,brush);
		this->line->setPalette(palette);
	}
	else
	{
		QColor color(255,255,255);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->line->palette();
		palette.setBrush(QPalette::Base,brush);
		this->line->setPalette(palette);	
	}
}

void SimonListWidget::showAllEntries()
{
	int i = 0;
	for (i = 0; i<this->count(); i++)
	{
		this->item(i)->setHidden(false);
	}	
}


void SimonListWidget::keyPressEvent ( QKeyEvent * event )
{
	
	if (event->key()==Qt::Key_Return && !this->line->isHidden())        //i have no notion, why i have to add 1 to the e->key(), that it gaves the same value as Qt::Key_Enter
	{
		this->selectItem();
		return;
	}
	
	//QString gugu = event->text();
	//QMessageBox::information(this,"",event->text());
	char c;
	QByteArray array = event->text().toLatin1();
	c = *array.data();
	//QMessageBox::information(this,"","GO GO GO");
	if(!((c<=126) && (c>=33)))
	{	
		return;
	}	
	// 65 122
	if(c == '\n')
	{
		return;
	}
	if(this->line->isHidden())
	{
		showLineEdit();
		this->line->setText(event->text());
		
	}
}

