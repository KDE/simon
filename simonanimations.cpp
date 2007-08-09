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
// 	qDebug() << ui.pbAddWord->width();
// 	qDebug()  << ui.pbEditWordList->width();
 	setUpdatesEnabled(false);
	if (currentSizeW <= 200) {
		tresizeButtons->stop();
		qDebug() << currentSizeW;
	}
	currentSizeW = currentSizeW*0.99;
	currentSizeH=currentSizeH*0.95;
	
	QSize newIconSize = QSize((currentSizeW-100)/5+8, (currentSizeW-100)/5+8);
	float newFontSize = round(((float)currentSizeW/(float)22)+3);
	for (int i=0; i < animatedButtons.size(); i++)
	{
// 		animatedButtons.at(i)->resize((int) round(currentSizeW), (int) round(currentSizeH));
		test->resize((int) round(currentSizeW*2.3), (int) round(currentSizeH*2.3));
		QFont f = animatedButtons.at(i)->font();
		f.setPointSizeF(newFontSize);
		animatedButtons.at(i)->setIconSize(newIconSize);
		animatedButtons.at(i)->setFont(f);
	}
 	setUpdatesEnabled(true);
// 	qDebug() << test->height();
// 	qDebug() << ui.pbAddWord->width();
// 	qDebug()  << ui.pbEditWordList->width();
// 	tresizeButtons->stop();
}

void SimonView::startTransformToBusy()
{
// 	currentSizeW = ui.pbAddWord->width();
// 	currentSizeH = ui.pbAddWord->height();

// 	for (int i=0; i < animatedButtons.count(); i++)
// 	{
// 		laBt = animatedButtons.at(i)->layout();
// 		laBt->removeWidget(animatedButtons.at(i));
// 	}
	ui.vboxLayout1->removeWidget(ui.pbAddWord);
	ui.vboxLayout1->removeWidget(ui.pbEditWordList);
	ui.vboxLayout2->removeWidget(ui.pbTrain);
	ui.vboxLayout2->removeWidget(ui.pbRunProgram);

	buttonMover->addWidget(ui.pbAddWord, 0,0);
	buttonMover->addWidget(ui.pbEditWordList, 1,0);
	buttonMover->addWidget(ui.pbTrain, 0,1);
	buttonMover->addWidget(ui.pbRunProgram, 1,1);
	buttonMover->setHorizontalSpacing(5);
	buttonMover->setVerticalSpacing(2);
	test = new QWidget(this);
	test->resize(ui.pbRunProgram->x()+ui.pbRunProgram->width() - ui.pbAddWord->x()+15, 
			ui.pbRunProgram->y()+ui.pbRunProgram->height() - ui.pbAddWord->y()+15);
	test->move(ui.pbAddWord->x()-9, ui.pbAddWord->y()-20);
	test->setLayout(buttonMover);

	currentSizeW = ((float)test->width())/2.5f;
	currentSizeH = ((float)test->height())/2.5f;
	test->show();
	//buttonMover->setColumnMinimumWidth(0,1000);
	tresizeButtons->start(40);
}
