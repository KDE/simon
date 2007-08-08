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
	if (currentSizeW <= 200) {
		tresizeButtons->stop();
		qDebug() << currentSizeW;
	}
	currentSizeW = currentSizeW*0.95;
	currentSizeH=currentSizeH*0.95;;
	
	QSize newIconSize = QSize((currentSizeW-100)/5+16, (currentSizeW-100)/5+16);
	float newFontSize = ((float)(currentSizeW/20))+(float)4;
	for (int i=0; i < animatedButtons.size(); i++)
	{
		animatedButtons.at(i)->resize((int) round(currentSizeW), (int) round(currentSizeH));
		QFont f = animatedButtons.at(i)->font();
		f.setPointSizeF(newFontSize);
// 		QIcon icon = animatedButtons.at(i)->icon();
		animatedButtons.at(i)->setIconSize(newIconSize);
		animatedButtons.at(i)->setFont(f);
	}
 	setUpdatesEnabled(true);
}

void SimonView::startTransformToBusy()
{
	//QMessageBox::information(this, "hallo", "dudu");
	currentSizeW = ui.pbAddWord->width();
	currentSizeH = ui.pbAddWord->height();

// 	for (int i=0; i < animatedButtons.count(); i++)
// 	{
// 		laBt = animatedButtons.at(i)->layout();
// 		laBt->removeWidget(animatedButtons.at(i));
// 	}
	ui.vboxLayout1->removeWidget(ui.pbAddWord);
	ui.vboxLayout1->removeWidget(ui.pbEditWordList);
	ui.vboxLayout4->removeWidget(ui.pbTrain);
	ui.vboxLayout4->removeWidget(ui.pbRunProgram);

	buttonMover->addWidget(ui.pbAddWord, 0,0);
	buttonMover->addWidget(ui.pbEditWordList, 0,1);
	buttonMover->addWidget(ui.pbTrain, 1,0);
	buttonMover->addWidget(ui.pbRunProgram, 1,1);
	//buttonMover->setColumnMinimumWidth(0,1000);
	tresizeButtons->start(40);
}
