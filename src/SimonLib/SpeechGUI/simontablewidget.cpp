#include "simontablewidget.h"
#include <QKeyEvent>
#include <QModelIndex>
#include <KLineEdit>


//TODO-List
//SimonTableWidget STW
// Arbeitsliste
//   -) LineEdits sollen so angeordnet werden, dass in jeder Spalte unten am Rand des STW erscheinen. Diese sollen bei veränderung der Spaltenbreite ihre Breite auch ändern
//       und sich anpassen........OK bis auf Spaltenbreitebänderung
//   -) Bei der Registrierung der STW sollen das STW selbst mittels dem Buddy, und weiters auch alle Spalten mit dessen Namen registriert werden (z.B.: "simon Hosts Port")
//       für dies soll eine eigene Funktion bei der Registrierung implementiert werden.
//   -)  sollte nur der Buddy angesprochen werden, sollen alle LineEdits angezeigt werden, aber kein Fokus gesetzt werden.................OK
//       der aufruf der eigenen Spalten erfolgt dann über z.B.:"simon Hosts Port"
//   -)  der direkter aufruf einer Spalte mittels z.B.: "simon Hosts Port" soll auch alle LineEdits anzeigen, und den Fokus für das gewünschte LineEdit setzen.
//   -) Die Elemente sollen Buchstabe für Buchstabe bei der eingabe gefiltert werden........................................................................................................................OK
//   -) Abgeschlossen wird das Filtern durch das drücken der Enter Taste. ...........................................................................................................................OK
//      bei einem übergebliebenen Element soll  dies selectiert werden, alle anderen wieder angezeigt, und zum gewünschten gescrollt werden.....OK
//      bei mehreren soll das erste in der liste selectiert werden, -"-.......................................................................................................................................OK
//   -) sollte kein Element mit der Suchabfrage übereinstimmen, soll das selectierte LineEdit rot markiert werden.
//   -) Bei verlieren des Focus soll die sortierung abgebrochen werden. (außer der Focus bleibt innerhalb der Lineedits, oder dem STW)
//
//  
//__________-
//
//   -) ist nun soweit, das mit label name und header namen funkt aufgerufen wird.



SimonTableWidget::SimonTableWidget( QWidget * parent) : QTableWidget(parent)
{
	this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
	this->redFlag = true;

}

SimonTableWidget::~SimonTableWidget()
{
	
}
	
/**
 * \brief if the Windowsize changes, this methode  will be called
 * \author Phillip Goriup
 *  
 */
void SimonTableWidget::resizeEvent(QResizeEvent * event)
{
	//this->moveLineEdit();
}

/**
 * \brief move the LineEdits to the new positions
 * \author Phillip Goriup
 *  
 */
void SimonTableWidget::moveLineEdit()
{
	if(linevector.at(0))
	{
		int i = 0;
		int difference = 23;
		for (; i<=linevector.count()-1 ;  i++)
		{
			linevector.at(i)->move(difference, this->height() - linevector.at(i)->height() - 10);
			linevector.at(i)->setMinimumWidth(this->columnWidth(i));
			linevector.at(i)->setMaximumWidth(this->columnWidth(i));
			linevector.at(i)->show();
			difference = difference + this->columnWidth(i);
		}

	}
	
}

/**
 * \brief calls the different methodes to initialize the LineEdits
 * \author Phillip Goriup
 *  
 */
void  SimonTableWidget::initLineEdit(QString action)
{
	if (this->linevector.isEmpty())
	{	
		this->count = this->columnCount(); 
		this->fillVector();
	}

	this->moveLineEdit();
	this->showLineEdit();

	for (int i = 0; i<this->columnCount(); i++)
		{
			if (action.contains(this->horizontalHeaderItem (i)->text(),Qt::CaseSensitive))
				this->linevector.at(i)->setFocus();
		}
}

/**
 * \brief shows all createt LineEdits
 * \author Phillip Goriup
 * 
 *
 */
void SimonTableWidget::showLineEdit()
{
	for (int i = 0; i<this->count; i++)
	{
		//this->linevector.at(i)->setText("");
		this->linevector.at(i)->show();
	}    

}

/**
 * \brief initializes the LineEdits
 * \author Phillip Goriup
 * initializes the LineEdits including the columncount from the SimonTableWidget.
 * safes the created LineEdits to an Vector
 */
void SimonTableWidget::fillVector()
{
	this->linevector.clear();
	for (int i = 0; i<this->columnCount(); i++)
	{
		SimonLineEdit *temp = new SimonLineEdit(this);
		temp->setObjectName (QString::number(i)); 
		this->linevector.append(temp) ;
		this->linevector.at(i)->hide();
		connect(linevector.at(i),SIGNAL(textChanged(QString)),this,SLOT(filterEntries(QString)));
		connect(linevector.at(i),SIGNAL(returnPressed () ),this,SLOT(selectItem()));
		connect(linevector.at(i),SIGNAL(checkFocus()),this,SLOT(checkFocus()));
	}     
}

/**
 * \brief filters the Entries 
 * \author Phillip Goriup
 * filters the entries with the Text in the Lineedits
 */
void SimonTableWidget::filterEntries(QString text)
{
	this->redFlag = true;
	SimonLineEdit* temp=0;
	
	for (int i = 0; i<this->columnCount(); i++)
	{
		if(this->linevector.at(i)->hasFocus ())
			temp = this->linevector.at(i);
	}
	this->showEntries();
	this->returnFilterEntries(temp);
	if (text.isEmpty())
	{
		this->redFlag = false;	
		controlRedFlag(temp->objectName().toInt());
		return;
	}
	
	this->invisibleRows(temp, text);
	
	
	
}

/**
 * \brief 
 * \author Phillip Goriup
 * 
 */
void SimonTableWidget::returnFilterEntries(SimonLineEdit* temp)
{
	//int index = this->linevector. indexOf(temp);

	for (int i = 0; i<this->linevector.count(); i++)
	{
		if (! linevector.at(i)->text().isEmpty())
		{
			QString text = this->linevector.at(i)->text();
			this->invisibleRows(linevector.at(i), text);
		}
		else
		{
			
		}
		this->controlRedFlag(i);
	}
}

/**
 * \brief 
 * \author Phillip Goriup
 * 
 */
void SimonTableWidget::invisibleRows(SimonLineEdit * temp, QString text)
{
	this->redFlag = true;
	int i = 0;
	for (; i<this->rowCount(); i++)
	{
		if(this->item(i,temp->objectName().toInt())->text().startsWith(text, Qt::CaseInsensitive) && !this->isRowHidden(i))
		{
			this->redFlag = false;
			this->setRowHidden(i,false);
		}
		else
		{
			this->setRowHidden(i,true);
		}
	}
}

/**
 * \brief shows all entries
 * \author Phillip Goriup
 * 
 */
void SimonTableWidget::showEntries()
{
	for (int i = 0; i<this->rowCount(); i++)
	{
		this->setRowHidden ( i, false ) ;
	}	
}

/**
 * \brief controls the RedFlag
 * \author Phillip Goriup
 * if the RedFlag is True, the color of the LineEdit will be set to red. else it will be set to white
 */
void SimonTableWidget::controlRedFlag(int lineValue)
{
	if (this->redFlag)
	{
		QColor color(224,67,67);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->linevector.at(lineValue)->palette();
		palette.setBrush(QPalette::Base,brush);
		this->linevector.at(lineValue)->setPalette(palette);
	}
	else
	{
		QColor color(255,255,255);
		QBrush brush(color, Qt::SolidPattern);
		QPalette palette = this->linevector.at(lineValue)->palette();
		palette.setBrush(QPalette::Base,brush);
		this->linevector.at(lineValue)->setPalette(palette);	
	}
}


SimonLineEdit* SimonTableWidget::getFocusedLineEdit()
{
	SimonLineEdit* temp=0;
	
	for (int i = 0; i<this->columnCount(); i++)
	{
		if(this->linevector.at(i)->hasFocus ())
			temp = this->linevector.at(i);
	}
	return temp;
}


/**
 * \brief selects the item after filtering
 * \author Phillip Goriup
 * selects the item that is on top of the list after filtering with the lineedits
 */
QTableWidgetItem* SimonTableWidget::selectItem()
{
	QTableWidgetItem * item = new QTableWidgetItem();
	for (int i = 0; i<this->rowCount(); i++)
	{
		if(!this->isRowHidden(i))
		{
			item = this->itemAt(i,0);
			return item;
		}
	}
	return item;
}


void SimonTableWidget::invisibleLineEdits()
{
	QTableWidgetItem * item = this->selectItem();
	this->showEntries();
	this->setFocus();
	this->setCurrentItem(item);
	this->scrollToItem(item,QAbstractItemView::PositionAtTop);
	
	for (int i = 0; i<=linevector.count()-1 ;  i++)
	{
		linevector.at(i)->hide();
	}
	linevector.clear();
	
}


void SimonTableWidget::keyPressEvent(QKeyEvent *e)
{
	
	if (e->key()==Qt::Key_Return)        //i have no notion, why i have to add 1 to the e->key(), that it gaves the same value as Qt::Key_Enter
	{
		this->invisibleLineEdits();
		emit returnPressed();
	}
}


void SimonTableWidget::focusOutEvent ( QFocusEvent * event )
{
	if(!linevector.count() == 0)
		if(!linevector.at(0)->isVisible())
		{
			this->checkFocus();
		}
		else
			event->ignore();
	else
		event->ignore();
}

void SimonTableWidget::checkFocus()
{
	
		bool invisibleFlag = false;
		for (int i = 0; i<=linevector.count()-1 ;  i++)
			{
				if(linevector.at(i)->hasFocus())
					 invisibleFlag = true;
			}
		if (this->hasFocus())
			invisibleFlag = true;
		
		if (!invisibleFlag)
			this->invisibleLineEdits();
	
}

