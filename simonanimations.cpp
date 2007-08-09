
#include "simonview.h"


#ifdef ANIMATIONS

#include <QMessageBox>
#include <QPushButton>
#include <QVector>
#include <QFont>
#include <QLayout>
#include <QCoreApplication>

void SimonView::setupAnimations()
{
	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
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
	if (currentSizeW <= this->width()-300	)
		wScaleDone=true;
	if (currentSizeH <= 90)
		hScaleDone=true;
	if (!logoScaleDone && (ui.lbLogo->height() >= 97))
		logoScaleFactor = 0.95f;
	else { logoScaleDone=true; }
	
	if (!xMoveDone && (test->x() > 60)) { currentMoveX = 1; }
	else { xMoveDone = true; currentMoveX=0; }

	if (!yMoveDone && (test->y() > 15)) { currentMoveY = 13; }
	else { yMoveDone = true; currentMoveY=0; }

	if (hScaleDone && wScaleDone && xMoveDone && yMoveDone && logoScaleDone)
	{ tresizeButtons->stop(); setButtonsBusy(); viewBusy=true; return; }

 	setUpdatesEnabled(false);


	if (!logoScaleDone)
	{
		ui.lbLogo->setMaximumSize(
			ui.lbLogo->size()*logoScaleFactor);
	}

	if (!wScaleDone)
		currentSizeW = currentSizeW-23;    // *0.94; //
	if (!hScaleDone)
		currentSizeH=currentSizeH*0.91;      // *0.95;

	
	int iconsize = round(((currentSizeH/2.5f))/5+8);
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

	test->move((int) round(test->x()+(currentMoveX*moveXDirection)),
			(int) round(test->y()+(currentMoveY*moveYDirection)));
 	setUpdatesEnabled(true);
 	QCoreApplication::processEvents();
}

void SimonView::startTransformToBusy()
{
// 	ui.lbLogo->resize(10,10);
	ui.vboxLayout->removeWidget(ui.wButtonWidget);
	test = ui.wButtonWidget;

	currentMoveX=0;
	currentMoveY=0;
	logoScaleFactor=0;
	logoScaleDone=xMoveDone=yMoveDone=hScaleDone=wScaleDone=false;
	
	moveXDirection=-1;
	moveYDirection=-1;
	logoScaleDirection=-1;
	currentSizeW = test->width();
	currentSizeH = test->height();
	tresizeButtons->start(40);
}

#endif


void SimonView::setButtonsBusy()
{
 	setUpdatesEnabled(false);

	((QGridLayout*) ui.wButtonWidget->layout())->setVerticalSpacing(1);
	ui.wButtonWidget->setMinimumHeight(89);
	ui.wButtonWidget->setMaximumHeight(89);
	ui.wButtonWidget->setMinimumWidth(this->width()-309);
	ui.wButtonWidget->setMaximumWidth(this->width()-309);
	ui.lbLogo->setMaximumWidth(167);
	ui.lbLogo->setMaximumHeight(94);
	ui.vboxLayout->removeWidget(ui.wButtonWidget);

	QSize newIconSize = QSize(15, 15);
	QFont f = ui.pbAddWord->font();
	f.setPointSizeF(15.6287f);
	ui.pbAddWord->setIconSize(newIconSize);
	ui.pbAddWord->setFont(f);
	ui.pbEditWordList->setIconSize(newIconSize);
	ui.pbEditWordList->setFont(f);
	ui.pbTrain->setIconSize(newIconSize);
	ui.pbTrain->setFont(f);
	ui.pbRunProgram->setIconSize(newIconSize);
	ui.pbRunProgram->setFont(f);

	ui.hboxLayout1->insertWidget(0,ui.wButtonWidget);

	viewBusy = false;
 	setUpdatesEnabled(true);
}

