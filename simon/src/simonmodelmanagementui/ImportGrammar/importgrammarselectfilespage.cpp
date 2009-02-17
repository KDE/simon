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


#include "importgrammarselectfilespage.h"
#include <KEditListBox>
#include <kdeversion.h>
#include <QTextCodec>
#include <QStringList>

ImportGrammarSelectFilesPage::ImportGrammarSelectFilesPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Input Files"));
	ui.setupUi(this);

#if KDE_IS_VERSION(4,0,80)
	ui.elbFiles->setCustomEditor(*(new KEditListBox::CustomEditor(ui.urFileToAdd, ui.urFileToAdd->lineEdit())));
#endif
	
	registerField("files*", ui.elbFiles, "items", SIGNAL(changed()));
	registerField("encoding*", ui.cbEncoding, "currentText", SIGNAL(currentIndexChanged(int)));
	registerField("includeUnknown", ui.cbIncludeUnknown);
}

void ImportGrammarSelectFilesPage::initializePage()
{
	ui.cbEncoding->addItem(i18n("Automatic"));
	QList<QByteArray> availableCodecs = QTextCodec::availableCodecs();
	QStringList encodings;
	foreach (const QByteArray& codec, availableCodecs)
		encodings << codec;
	encodings.sort();
	ui.cbEncoding->addItems(encodings);
}


void ImportGrammarSelectFilesPage::cleanupPage()
{
	ui.elbFiles->clear();
}

ImportGrammarSelectFilesPage::~ImportGrammarSelectFilesPage()
{
}
