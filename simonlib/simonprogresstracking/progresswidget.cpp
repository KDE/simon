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
#include <KLocale>

ProgressWidget::ProgressWidget(QPointer<Operation> O, QWidget* parent) : QWidget(parent),
	op(O)
{
	KLocale::setMainCatalog("simonlib");

	if (op)
	{
		name = new QLabel(op->name(), this);
		QFont font(name->font());
		font.setBold(true);
		name->setFont(font);
		currentAction = new QLabel(op->currentAction(), this);
	
		bar = new QProgressBar(this);
		bar->setValue(op->currentProgress());
		bar->setMaximum(op->maxProgress());
	
		cancelButton = new KPushButton(KIcon("process-stop"), i18n("Cancel"), this);
		if (op->isAtomic()) cancelButton->setEnabled(false);
		
		QHBoxLayout *hBox = new QHBoxLayout();
		hBox->addWidget(currentAction);
		hBox->addWidget(bar);
		hBox->addWidget(cancelButton);

		QVBoxLayout *vBox = new QVBoxLayout(this);
		vBox->addWidget(name);
		vBox->addLayout(hBox);
		vBox->addWidget(new QSplitter(this));
		connect(cancelButton, SIGNAL(clicked()), op, SLOT(cancel()));
	}
}


void ProgressWidget::update()
{
	currentAction->setText(op->currentAction());
	bar->setValue(op->currentProgress());
	bar->setMaximum(op->maxProgress());

	if (!op->isRunning())
		cancelButton->setDisabled(true);
}


ProgressWidget::~ProgressWidget()
{
}


