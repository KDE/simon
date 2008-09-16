#include "simonslider.h"
#include <QMessageBox>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <KLineEdit>



SimonSlider::SimonSlider( QWidget * parent ) : QSlider(parent)
{	
	this->lePlusMinus = new KLineEdit(dynamic_cast<QWidget*>(this->parent()));
	this->lePlusMinus->setInputMask ("99");
	this->lePlusMinus->setVisible(false);
	this->setMinimum(0);
	this->setMaximum(100);
	this->lePlusMinus->setMaximumHeight( 18 ) ;
	this->lePlusMinus->setMinimumHeight ( 18 ) ;
	QColor color(255,255,255,127);
	QBrush brush(color, Qt::SolidPattern);
	QPalette palette = this->lePlusMinus->palette();
	palette.setBrush(QPalette::Base,brush);
	this->lePlusMinus->setPalette(palette);
	
	connect(lePlusMinus,SIGNAL(returnPressed()),this,SLOT(doCommand())); 
}	

SimonSlider::~SimonSlider()
{
	lePlusMinus->deleteLater();
}

void SimonSlider::changeValue()
{
	this->lePlusMinus->setVisible(true);
	this->lePlusMinus->move(dynamic_cast<QWidget*>(lePlusMinus->parent())->width()-lePlusMinus->width(),10);
	this->lePlusMinus->setFocus();
	this->lePlusMinus->clear();
}

void SimonSlider::doCommand()
{
	if ((this->lePlusMinus->text().toInt() >100) || (this->lePlusMinus->text().toInt() < 0))
	{	
		QMessageBox::information(this,"Ungültiger Wert","Der von Ihnen eingegebene Wert ist auserhalb des Bereiches der eingestellt werden kann.\nBitte geben sie einen Wert zwischen\n\t0 und 100\nein.");
		return;
	}
		this->setValue(this->lePlusMinus->text().toInt()); 
		this->lePlusMinus->setVisible(false);
}


