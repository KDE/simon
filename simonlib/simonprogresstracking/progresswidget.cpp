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
#include "progresswidget.h"

#include "operation.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QProgressBar>
#include <KPushButton>
#include <KLocalizedString>

ProgressWidget::ProgressWidget(QPointer<Operation> op, QWidget* parent): QWidget(parent)
{
	this->op = op;

	name = new QLabel(op->name(), this);
	QFont font(name->font());
	font.setBold(true);
	name->setFont(font);
	currentAction = new QLabel(op->currentAction(), this);

	bar = new QProgressBar(this);
	bar->setValue(op->currentProgress());
	bar->setMaximum(op->maxProgress());

	cancelButton = new KPushButton(KIcon("process-stop"), i18n("Abbrechen"), this);
	
	QHBoxLayout *hBox = new QHBoxLayout();
	hBox->addWidget(currentAction);
	hBox->addWidget(bar);
	hBox->addWidget(cancelButton);
	if (op->isAtomic()) cancelButton->setEnabled(false);

	QVBoxLayout *vBox = new QVBoxLayout(this);
	vBox->addWidget(name);
	vBox->addLayout(hBox);
	vBox->addWidget(new QSplitter(this));
	
//	connect(op, SIGNAL(changed()), this, SLOT(update()));
}


void ProgressWidget::update()
{
	currentAction->setText(op->currentAction());
	bar->setValue(op->currentProgress());
	bar->setMaximum(op->maxProgress());
}


ProgressWidget::~ProgressWidget()
{
}


