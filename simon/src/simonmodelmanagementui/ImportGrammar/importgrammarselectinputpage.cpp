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

#include "importgrammarselectinputpage.h"
#include <QTextCodec>
#include <QStringList>
#include <KEditListBox>
#include <kdeversion.h>

ImportGrammarSelectInputPage::ImportGrammarSelectInputPage(QWidget* parent): QWizardPage(parent)
{
  setTitle(i18n("Input"));
  ui.setupUi(this);

  #if KDE_IS_VERSION(4,0,80)
  ui.elbFiles->setCustomEditor(*(new KEditListBox::CustomEditor(ui.urFileToAdd, ui.urFileToAdd->lineEdit())));
  #endif

  registerField("inputIsText", ui.rbText);

  registerField("files", ui.elbFiles, "items", SIGNAL(changed()));
  registerField("encoding", ui.cbEncoding, "currentText", SIGNAL(currentIndexChanged(int)));
  registerField("includeUnknown", ui.cbIncludeUnknown);
  registerField("grammarInputText", ui.teText, "plainText", SIGNAL(textChanged()));

  connect(ui.teText, SIGNAL(textChanged()), this, SIGNAL(completeChanged()));
  connect(ui.elbFiles, SIGNAL(changed()), this, SIGNAL(completeChanged()));
  connect(ui.rbText, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
  ui.wgFileImport->hide();
  ui.rbText->toggle();
}


void ImportGrammarSelectInputPage::initializePage()
{
  ui.cbEncoding->addItem(i18nc("Selection box for the encoding: Determine the encoding automatically", "Automatic"));
  QList<QByteArray> availableCodecs = QTextCodec::availableCodecs();
  QStringList encodings;
  foreach (const QByteArray& codec, availableCodecs)
    encodings << codec;
  encodings.sort();
  ui.cbEncoding->addItems(encodings);
}


bool ImportGrammarSelectInputPage::isComplete() const
{
  if (ui.rbFiles->isChecked())
    return ui.elbFiles->count() > 0;
  else
    return !ui.teText->toPlainText().isEmpty();
}


void ImportGrammarSelectInputPage::cleanupPage()
{
  ui.elbFiles->clear();
}


ImportGrammarSelectInputPage::~ImportGrammarSelectInputPage()
{
}
