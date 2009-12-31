/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
 
#include "compositeprogresswidget.h"
#include "progresswidget.h"

#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

#include <KLocalizedString>
#include <KIcon>
#include <KPushButton>
#include <KLocale>
#include <KDebug>

CompositeProgressWidget::CompositeProgressWidget(QWidget* parent): QWidget(parent)
{
	KLocale::setMainCatalog("simonlib");
	this->designatedParent = parent;
	statusLabel = new QLabel(this);
	bar = new QProgressBar(this);
	popupWidget = new QFrame(parent);
	popupWidget->setLayout(new QVBoxLayout());
	popupWidget->move(this->pos());
	popupWidget->hide();
	popupWidget->setAutoFillBackground(true);
	popupWidget->setFrameStyle(QFrame::Box);
	
	togglePopup = new KPushButton(KIcon("arrow-up"), i18n("Details"), this);
	togglePopup->setCheckable(true);
	togglePopup->setEnabled(false);
	
	connect(togglePopup, SIGNAL(toggled(bool)), this, SLOT(showDetails(bool)));
	
	QHBoxLayout *lay = new QHBoxLayout(this);
	lay->addWidget(statusLabel);
	lay->addWidget(bar);
	lay->addWidget(togglePopup);
	setLayout(lay);
}

void CompositeProgressWidget::showDetails(bool show)
{
	if (show)
	{
		popupWidget->resize(popupWidget->sizeHint());
		QPoint point = mapTo(designatedParent, pos());
		
		popupWidget->move(pos().x()+width()-popupWidget->width(),
				point.y()-popupWidget->height());
		popupWidget->show();
	} else popupWidget->hide();
}

void CompositeProgressWidget::display(OperationList operations)
{	
	QString status;
	int now = 0;
	int max = 0;

	int noProgressData=0;
			
	switch (operations.count())
	{
		case 0:
		{
			status = i18n("Finished");
			now=max=1;
		}
		break;
		case 1:
		{
			QPointer<Operation> op = operations.at(0);
			if (!op) break;;
			status = op->name();
			now = op->currentProgress();
			max = op->maxProgress();
		}
		break;
		default:
		{
			now = max =0;
			foreach (QPointer<Operation> op, operations)
			{
				if (!op) continue;

				status += op->name()+", ";
				if (op->maxProgress() != 0)
				{
					now += op->currentProgress();
					max += op->maxProgress();
				} else {
					noProgressData++;
				}
			}

			//get rid of last ", "
			status = status.left(status.count()-2);

			if (noProgressData != 0)
			{	//there are processes without progress data
				if (operations.count() == noProgressData)
				{
					now=max=0;
				} else {
					int averageMax = max / (operations.count() - noProgressData);
					max += (averageMax*noProgressData);
				}
			}
		}
	}

	foreach (ProgressWidget *widget, progressWidgets)
	{
		if ((!widget->operation()) || (!operations.contains(widget->operation())))
		{
			popupWidget->layout()->removeWidget(widget);
			progressWidgets.removeAll(widget);
			widget->deleteLater();
		}
		else {
			operations.removeAll(widget->operation());
			widget->update();
		}
	}

	foreach (QPointer<Operation> op, operations)
	{
		if (!op) continue;

		ProgressWidget *widget = new ProgressWidget(op, ProgressWidget::Compact, designatedParent);
		popupWidget->layout()->addWidget(widget);
		progressWidgets << widget;
	}
	

	if (progressWidgets.count() > 0)
	{
		togglePopup->setEnabled(true);
		if (togglePopup->isChecked())
			showDetails(true); //resize / move
	} else {
		showDetails(false); //hide empty widget
		togglePopup->setEnabled(false);
		togglePopup->setChecked(false);
	}
	
	if (status.size() > 60)
		status = "..."+status.right(60);
	statusLabel->setText(status);
	bar->setValue(now);
	bar->setMaximum(max);
}

void CompositeProgressWidget::moveEvent(QMoveEvent *)
{
	if (togglePopup->isChecked())
	{
		showDetails(true);
	}
}


CompositeProgressWidget::~CompositeProgressWidget()
{
}


