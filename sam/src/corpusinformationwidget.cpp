/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "corpusinformationwidget.h"

CorpusInformationWidget::CorpusInformationWidget(CorpusInformation*information, QWidget *parent) : QWidget(parent),
      m_information(information)
{
  ui.setupUi(this);

  ui.leTag->setText(information->tag());
  ui.teNotes->setPlainText(information->notes());
  ui.sbSamples->setValue(information->samples());
  ui.sbSpeakers->setValue(information->speakers());
}

/**
 * saves the current ui values to the information (member)
 */
void CorpusInformationWidget::submit()
{
  m_information->setTag(ui.leTag->text());
  m_information->setNotes(ui.teNotes->toPlainText());
  m_information->setSamples(ui.sbSamples->value());
  m_information->setSpeakers(ui.sbSpeakers->value());
}

