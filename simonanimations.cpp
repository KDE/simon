#include "simonview.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVector>
#include <QFont>
#include <QLayout>
#include <QCoreApplication>

void SimonView::setupAnimations()
{
	QObject::connect(ui.pbAddWord, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
	QObject::connect(ui.pbRunProgram, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
	QObject::connect(ui.pbTrain, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
	QObject::connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
	tresizeButtons = new QTimer(this);
	connect(tresizeButtons, SIGNAL(timeout()), this, SLOT(resizeButtons()));

	buttonMover = new QGridLayout();

	animatedButtons =  QVector<QPushButton*>();
	animatedButtons << ui.pbAddWord;
	animatedButtons << ui.pbEditWordList;
	animatedButtons << ui.pbTrain;
	animatedButtons << ui.pbRunProgram;
}

void SimonView::resizeButtons()
{
 	setUpdatesEnabled(false);
	currentSizeW = currentSizeW*0.98; //-2.5f;    // 
	currentSizeH=currentSizeH*0.963;      // *0.95;
	
	int iconsize = round(((currentSizeW/2.5f)-100)/5+8);
	QSize newIconSize = QSize(iconsize, iconsize);
	float newFontSize = (currentSizeW/(float) 1000)*19  + 5;

	test->resize((int) round(currentSizeW), (int) round(currentSizeH));
	QPushButton *tmp;
	for (int i=0; i < animatedButtons.size(); i++)
	{
		tmp = animatedButtons.at(i);
		QFont f = tmp->font();
		f.setPointSizeF(newFontSize);
		tmp->setFont(f);
		tmp->setIconSize(newIconSize);
	}

	if (currentSizeW <= 430) {
		tresizeButtons->stop();
		qDebug() << currentSizeW;
	} else if(currentSizeW >= 680)
	{
		currentMoveY+=1.3;
	} else if (currentSizeW <= 455)
	{
		currentMoveY -= 2.6;
	}
	qDebug() << tmp->y();

	test->move(test->x(), test->y()+(currentMoveY*moveYDirection));
 	setUpdatesEnabled(true);
}

void SimonView::startTransformToBusy()
{

// 	test = new QWidget(this);
// 	test->resize(ui.pbRunProgram->x()+ui.pbRunProgram->width() - ui.pbAddWord->x()+15, 
// 			ui.pbRunProgram->y()+ui.pbRunProgram->height() - ui.pbAddWord->y()+15);
// 	test->move(ui.pbAddWord->x()-9, ui.pbAddWord->y()-20);


// 	QLayout *lay = ui.wButtonWidget->layout();
	
// 	lay->removeWidget(ui.pbAddWord);
// 	lay->removeWidget(ui.pbEditWordList);
// 	lay->removeWidget(ui.pbTrain);
// 	lay->removeWidget(ui.pbRunProgram);

// 	buttonMover->addWidget(ui.pbAddWord, 0,0);
// 	buttonMover->addWidget(ui.pbEditWordList, 1,0);
// 	buttonMover->addWidget(ui.pbTrain, 0,1);
// 	buttonMover->addWidget(ui.pbRunProgram, 1,1);
// 	buttonMover->setHorizontalSpacing(5);

// 	test->setLayout(buttonMover);

	ui.vboxLayout->removeWidget(ui.wButtonWidget);
	test = ui.wButtonWidget;
	((QGridLayout*) test->layout())->setVerticalSpacing(1);

	currentMoveY=0;
	moveYDirection=-1;
	currentSizeW = test->width();
// 	qDebug() << currentSizeW;
	currentSizeH = test->height();
	test->show();
	//buttonMover->setColumnMinimumWidth(0,1000);
	tresizeButtons->start(40);
}
