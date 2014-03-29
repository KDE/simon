/*   Copyright (C) 2009-2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "turnconfiguration.h"
#include <simondialogengine/dialogturn.h>

#include <simonscenarios/scenario.h>

#include "createdialogcommandwidget.h"
#include "createtransitiondialog.h"

#include <QVariantList>
#include <QList>
#include <QString>
#include <QDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>

#include <kgenericfactory.h>
#include <KAboutData>
#include <KInputDialog>
#include <KMessageBox>
#include <KStandardDirs>

TurnConfiguration::TurnConfiguration(DialogTurn* _turn, QWidget* parent) :
	KDialog(parent),
  turn(_turn)
{
	ui.setupUi(mainWidget());

  connect(ui.pbAddPrompt, SIGNAL(clicked()), this, SLOT(addPrompt()));
  connect(ui.pbEditPrompt, SIGNAL(clicked()), this, SLOT(editPrompt()));
  connect(ui.sbPrompt, SIGNAL(valueChanged(int)), this, SLOT(displaySelectedText()));
  connect(this, SIGNAL(okClicked()), this, SLOT(save()));
  connect(this, SIGNAL(cancelClicked()), this, SLOT(forget()));

  displayCurrentTurn();
}

void TurnConfiguration::displayCurrentTurn()
{
  ui.leTurnName->setText(turn->getName());
  //TODO: display distractors
  displaySelectedText();
}

void TurnConfiguration::addPrompt()
{
	turn->addText("");
  updateTextSelector();
  ui.sbPrompt->setValue(ui.sbPrompt->maximum());
  displaySelectedText();
}

void TurnConfiguration::removePrompt()
{
	turn->removeText(ui.sbPrompt->value() - 1);
  displaySelectedText();
}

void TurnConfiguration::editPrompt()
{
  bool ok;
  QString text = KInputDialog::getMultiLineText(i18n("Text"), i18n("Enter the text to present to the user when this turn is entered:"), 
                                        turn->getRawText(ui.sbPrompt->value()-1), &ok);
  if (!ok) return;
  
  if (!turn->setRawText(ui.sbPrompt->value()-1, text))
    KMessageBox::sorry(this, i18n("Failed to update turn text."));

  displaySelectedText();
}

void TurnConfiguration::updateTextSelector()
{
  ui.sbPrompt->setMaximum(turn->getTextCount());
  
  displaySelectedText();
  ui.pbRemovePrompt->setEnabled(turn->getTextCount() > 1);
}

void TurnConfiguration::displaySelectedText()
{
  int textId = ui.sbPrompt->value()-1;
  kDebug() << "Getting text " << textId;
  ui.tePrompt->setText(turn->getRawText(textId));
}

void TurnConfiguration::forget()
{
	code = QDialog::Rejected;
}

void TurnConfiguration::save()
{
	code = QDialog::Accepted;
	turn->rename(ui.leTurnName->displayText());
}

void TurnConfiguration::textSilenceChanged()
{
}

void TurnConfiguration::textAnnounceRepeatChanged()
{
}

void TurnConfiguration::addExtractor()
{
}

void TurnConfiguration::removeExtractor()
{
}

void TurnConfiguration::init()
{
}

bool TurnConfiguration::deSerialize(const QDomElement&)
{
  return true;
}

QDomElement TurnConfiguration::serialize(QDomDocument *doc)
{
  return QDomElement();
}

void TurnConfiguration::defaults()
{
}

TurnConfiguration::~TurnConfiguration()
{
}

QString TurnConfiguration::getRepeatAnnouncement() const
{
  return QString();
}

QStringList TurnConfiguration::getRepeatTriggers() const
{
  return QStringList();
}

bool TurnConfiguration::getRepeatOnInvalidInput() const
{
  return true;
}
