
#include "simonview.h"


#ifdef ANIMATIONS

#include <QMessageBox>
#include <QPushButton>
#include <QVector>
#include <QFont>
#include <QLayout>
#include <QCoreApplication>
#include "inlinewidget.h"

void SimonView::setupAnimations()
{
// 	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
// 	QObject::connect(ui.pbTrain, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));
// 	QObject::connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(startTransformToBusy()));

	tresizeButtons = new QTimer(this);
	connect(tresizeButtons, SIGNAL(timeout()), this, SLOT(resizeButtons()));

	animatedButtons =  QVector<QPushButton*>();
	animatedButtons << ui.pbAddWord;
	animatedButtons << ui.pbEditWordList;
	animatedButtons << ui.pbTrain;
	animatedButtons << ui.pbRunProgram;
}


void SimonView::resizeButtons()
{
	if (!wScaleDone && (((scaleXFactor < 1) && (currentSizeW <= this->width()-365)) ||
		((scaleXFactor > 1) && (currentSizeW >= this->width()-120)) || (scaleXFactor==1)))
		wScaleDone=true;

	if (!hScaleDone && (((currentSizeH <= 90) && (scaleYFactor < 1)) || 
		((scaleYFactor>1) && (currentSizeH > height()-260)) || (scaleYFactor==1)))
		hScaleDone=true;

	if (!logoScaleDone && ( ((logoScaleFactor<1) && (ui.lbLogo->height() <= 100)) ||  
		((logoScaleFactor>1) && (ui.lbLogo->height() >= 140)) ))
		logoScaleDone=true; 
	
	if (!xMoveDone && (((moveXDirection < 0) && (buttonMover->x() <= 60)) ||
		((moveXDirection > 0) && (buttonMover->x() >= 55)) || (moveXDirection==0)))
		xMoveDone = true;

	if (!yMoveDone && (((moveYDirection < 0) && (buttonMover->y() < 15)) ||
		((moveYDirection > 0) && (buttonMover->y() > 170)) || (moveYDirection==0)))
		yMoveDone = true;

	if (hScaleDone && wScaleDone && xMoveDone && yMoveDone && logoScaleDone)
	{ 
		tresizeButtons->stop(); 
		if (viewShouldBeBusy()) {
			setButtonsBusy();
			viewBusy=true;
		} else {
			setButtonsIdle();
			viewBusy = false;
		}
		return;
	}

 	setUpdatesEnabled(false);


	if (!logoScaleDone)
	{
		ui.lbLogo->setMaximumSize(ui.lbLogo->size()*logoScaleFactor);
	}

	if (!wScaleDone)
		currentSizeW = currentSizeW*scaleXFactor; //-23;    // *0.94; //
	if (!hScaleDone)
		currentSizeH=currentSizeH*scaleYFactor; //*0.91;      // *0.95;

	if (!hScaleDone || !wScaleDone)
		buttonMover->resize((int) round(currentSizeW), (int) round(currentSizeH));
	
	resizeMainButtonContentsToWindow();

	if (!yMoveDone)
		buttonMover->move(buttonMover->x(),
			(int) round(buttonMover->y()+(currentMoveY*moveYDirection)));

	if (!xMoveDone)
		buttonMover->move((int) round(buttonMover->x()+(currentMoveX*moveXDirection)),
			buttonMover->y());

 	setUpdatesEnabled(true);
 	QCoreApplication::processEvents();
}

void SimonView::startTransformToBusy()
{
	ui.vboxLayout->removeWidget(buttonMover);

	logoScaleDone=xMoveDone=yMoveDone=hScaleDone=wScaleDone=false;
	
	moveXDirection=-1;
	moveYDirection=-1;
	logoScaleDirection=-1;
	currentMoveX=1;
	currentMoveY=13;
	scaleXFactor = 0.95f;
	scaleYFactor = 0.91f;
	logoScaleFactor = 0.95f;
	currentSizeW = buttonMover->width();
	currentSizeH = buttonMover->height();
	tresizeButtons->start(40);
}

void SimonView::startTransformToIdle()
{
	ui.hboxLayout1->removeWidget(buttonMover);

	logoScaleDone=xMoveDone=yMoveDone=hScaleDone=wScaleDone=false;
	
	moveXDirection=1;
	moveYDirection=1;
	logoScaleDirection=1;
	currentMoveX=1;
	currentMoveY=17;
	scaleXFactor = 1.036f;
	scaleYFactor = 1.09f;
	logoScaleFactor = 1.05f;
	buttonMover->setMaximumWidth(16777215);
	buttonMover->setMaximumHeight(16777215);

	currentSizeW = buttonMover->width();
	currentSizeH = buttonMover->height();
	tresizeButtons->start(40);
}

#endif


void SimonView::inlineButtonClicked()
{
	if (viewShouldBeBusy())
	{
		#ifdef ANIMATIONS
		if (!viewBusy || tresizeButtons->isActive())
		{
			startTransformToBusy();
		}
		#endif
		#ifndef ANIMATIONS	
		if (!viewBusy)
		{
			setButtonsBusy();
		}
		#endif
	} else  {
		#ifdef ANIMATIONS
		if (viewBusy || tresizeButtons->isActive()) {
			startTransformToIdle();
		}
		#endif
		#ifndef ANIMATIONS
		if (viewBusy) {
			setButtonsIdle();
		}
		#endif
	
	}
}

void SimonView::setButtonsBusy()
{
 	setUpdatesEnabled(false);

	ui.vboxLayout->removeWidget(buttonMover);
	ui.hboxLayout1->insertWidget(0,buttonMover);

	((QGridLayout*) buttonMover->layout())->setVerticalSpacing(1);
	buttonMover->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	buttonMover->resize(this->width()-330, 89);
	buttonMover->setMaximumWidth(this->width()-370);
	buttonMover->setMinimumWidth(this->width()-370);
	buttonMover->setMinimumHeight(89);

	
	ui.lbLogo->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	ui.lbLogo->resize(167, 94);
	ui.lbLogo->setMaximumWidth(167);
	ui.lbLogo->setMaximumHeight(94);

	resizeMainButtonContentsToWindow();

	viewBusy = false;
 	setUpdatesEnabled(true);

	viewBusy = true;
}

void SimonView::setButtonsIdle()
{
 	setUpdatesEnabled(false);

	ui.hboxLayout1->removeWidget(buttonMover);
	ui.vboxLayout->insertWidget(2,buttonMover);

	((QGridLayout*) buttonMover->layout())->setVerticalSpacing(3);

	QSizePolicy pol(QSizePolicy::Expanding,QSizePolicy::Expanding);
	pol.setVerticalStretch(1);
	buttonMover->setSizePolicy(pol);
	buttonMover->setMaximumWidth(16777215);
	buttonMover->setMaximumHeight(16777215);
	buttonMover->setMinimumWidth(0);

	
	ui.lbLogo->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));
	ui.lbLogo->resize(265, 149);
	ui.lbLogo->setMaximumWidth(16777215);
	ui.lbLogo->setMaximumHeight(16777215);


	viewBusy = false;
 	setUpdatesEnabled(true);
	QCoreApplication::processEvents();
	resizeMainButtonContentsToWindow();
}
