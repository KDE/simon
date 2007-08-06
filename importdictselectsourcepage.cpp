//
// C++ Implementation: importdictselectsourcepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictselectsourcepage.h"

#include <QLabel>
#include <QRadioButton>
#include <QVariant>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

ImportDictSelectSourcePage::ImportDictSelectSourcePage(QWidget* parent): QWizardPage(parent)
{
	setTitle("Typ auswählen");
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	QLabel *desc = new QLabel(this);
	desc->setText(tr("Simon kann zwei verschiedene Typen von Wörterbüchern importieren: HADIFIX-BOMP Wörterbücher und Wiktionary-Dumps.\n\nBitte selektieren Sie welchen Typ von Wörterbuch Sie importieren möchten:\n"));
	desc->setWordWrap(true);
	
	QRadioButton *hadifixBOMP=new QRadioButton(tr("HADIFIX-BOMP"), this);
	QRadioButton *wiktionary = new QRadioButton(tr("Wiktionary Wörterbuch"), this);
	
	registerField("hadifix", hadifixBOMP, "checked", SIGNAL(toggled(bool)));
	registerField("wiktionary", wiktionary, "checked", SIGNAL(toggled(bool)));

	lay->addWidget(desc);
	lay->addWidget(hadifixBOMP);
	lay->addWidget(wiktionary);
	setLayout(lay);

	hadifixBOMP->setChecked(true);
}

int ImportDictSelectSourcePage::getType()
{
	return (field("hadifix").toBool()) ? HADIFIXBOMP : WIKTIONARY;
}

int ImportDictSelectSourcePage::nextId() const
{
	if (field("hadifix").toBool())
	{
		return 2;
	} else return 3;
}

ImportDictSelectSourcePage::~ImportDictSelectSourcePage()
{
}


