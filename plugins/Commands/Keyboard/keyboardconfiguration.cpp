/*
 *   Copyright (C) 2009 Dominik Neumeister & Mario Strametz <neudob06@edvhtl.at>  <strmam06@htl-kaindorf.ac.at>
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

#include "keyboardconfiguration.h"
#include "keyboardcommandmanager.h"
#include "keyboardtab.h"
#include "keyboardsetcontainer.h"
#include "keyboardaddbuttondialog.h"

#include <QVariantList>
#include <kgenericfactory.h>
#include <QList>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>
#include <QString>
#include <QDialog>
#include <qinputdialog.h>
#include <QTableView>
#include <QDebug>


K_PLUGIN_FACTORY_DECLARATION(KeyboardCommandPluginFactory)

QPointer<KeyboardConfiguration> KeyboardConfiguration::instance;

KeyboardConfiguration::KeyboardConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("keyboard", ki18n( "Keyboard" ),
				      "0.1", ki18n("Input signes with ease"),
				      "accessories-calculator",
				      KeyboardCommandPluginFactory::componentData(),parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(KeyboardCommandPluginFactory::componentData(),"keyboardrc");

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
        connect(ui.tbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
	connect(ui.tbDeleteSet, SIGNAL(clicked()), this, SLOT(deleteSet()));
	connect(ui.tbAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
	connect(ui.tbDeleteTab, SIGNAL(clicked()), this, SLOT(deleteTab()));
        connect(ui.pbAddButton, SIGNAL(clicked()), this, SLOT(addButton()));
	connect(ui.pbDeleteButton, SIGNAL(clicked()), this, SLOT(deleteButton()));
	connect(ui.pbUpButton, SIGNAL(clicked()), this, SLOT(buttonUp()));
	connect(ui.pbDownButton, SIGNAL(clicked()), this, SLOT(buttonDown()));
        connect(ui.cbSets, SIGNAL(currentIndexChanged(int)), this, SLOT(cbSetsIndexChanged()));
        connect(ui.cbSets, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
        connect(ui.cbTabs, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTabDetail()));

	setContainer = new KeyboardSetContainer();
}

QString KeyboardConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Keyboard"));
}

void KeyboardConfiguration::addSet()
{	
        bool ok;
        QString inputText = QInputDialog::getText(this, "Add keyboard set", "Please enter the name of the new set:", QLineEdit::Normal, QString(), &ok);
	if (!inputText.isEmpty()) {
		if (!setContainer->createSet(inputText))
			KMessageBox::sorry(this, i18n("Failed to add set"));
		refreshCbSets();
		emit changed(true);
	}
}
void KeyboardConfiguration::deleteSet()
{	
        if(ui.cbSets->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please choose a set to be deleted"));
		return;
	}
	if (!setContainer->deleteSet(ui.cbSets->currentText()))
		KMessageBox::sorry(this, i18n("Couldn't delete set"));
	
	refreshCbSets();
	emit changed(true); //when the delete failed we should still write the internal structure
}

void KeyboardConfiguration::addTab()
{	
	if(ui.cbSets->currentIndex() == -1)  {
		KMessageBox::information(this, i18n("Please insert or select a set first"));
		return;
	}

	QString inputText = QInputDialog::getText(this, "Add keyboard tab","Please enter the name of the new tab:");

	if(!inputText.isEmpty()) {
		if (!setContainer->createTab(ui.cbSets->currentText(), inputText))
			KMessageBox::sorry(this, i18n("Failed to add tab"));
	}

	refreshCbTabs();
	emit changed(true);
}
void KeyboardConfiguration::deleteTab()
{	
	if(ui.cbSets->currentIndex() == -1)  {
		KMessageBox::information(this, i18n("Please select a set first"));
		return;
	}
	QString tab = ui.cbTabs->currentText();
	if (tab.isEmpty()) {
		KMessageBox::information(this, i18n("Please select the tab to be deleted"));
		return;
	}

	if (!setContainer->deleteTab(ui.cbSets->currentText(), tab))
		KMessageBox::sorry(this, i18n("Failed to delete tab"));

	refreshCbTabs();
	emit changed(true);
}

void KeyboardConfiguration::addButton()
{
	if(ui.cbSets->currentIndex() != -1)
	{
		if(ui.cbTabs->currentIndex() != -1)
		{
        		KeyboardAddButtonDialog *kab = new KeyboardAddButtonDialog(this);
      			KeyboardButton *kbb =  kab->addButton();
			if(kbb!=NULL)
			{
				/*ButtonTableModel *model = dynamic_cast<ButtonTableModel*>(ui.tvTabContent->model());
				kDebug() << "Is model model?";
				if (!model) return;
				kDebug() << "Yeah it is";
				model->addButton(ui.cbSets->currentIndex(), ui.cbTabs->currentIndex(), kbb);*/
			}
		}
		else
			KMessageBox::information(this, i18n("Please select a tab to which to add the new button"));
	}
	else
		KMessageBox::information(this, i18n("Please select a set to which to add the new button"));
}

void KeyboardConfiguration::deleteButton()
{
}

void KeyboardConfiguration::buttonUp()
{
}

void KeyboardConfiguration::buttonDown()
{
}

void KeyboardConfiguration::cbSetsIndexChanged()
{
	refreshCbTabs();
}


void KeyboardConfiguration::refreshCbSets()
{
	ui.cbSets->clear();

	ui.cbSets->addItems(setContainer->getAvailableSets());

	refreshCbTabs();
}

void KeyboardConfiguration::refreshCbTabs()
{
	ui.cbTabs->clear();
	if (ui.cbSets->currentIndex() == -1) {
		return;
	}

	QString currentSet = ui.cbSets->currentText();

	ui.cbTabs->addItems(setContainer->getAvailableTabs(currentSet));
	refreshTabDetail();
}

void KeyboardConfiguration::refreshTabDetail()
{
	if ((ui.cbSets->currentIndex() == -1) || (ui.cbTabs->currentIndex() == -1)) {
		ui.tvTabContent->setModel(NULL);
		return;
	}

	QString currentSet = ui.cbSets->currentText();
	QString currentTab = ui.cbTabs->currentText();
	ui.tvTabContent->setModel(setContainer->getTab(currentSet, currentTab));
}

void KeyboardConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());

        cg.sync();
	if (!setContainer->save()) {
		KMessageBox::sorry(this, i18n("Failed to save keyboard sets"));
		return;
	}


	emit changed(false);
}

void KeyboardConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void KeyboardConfiguration::load()
{
        Q_ASSERT(config);
	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Keyboard")));

	cg.sync();

	setContainer->clear();
	if (!setContainer->load()) {
		KMessageBox::sorry(this, i18n("Failed to load keyboard sets from storage"));
		return;
	}

	refreshCbSets();

	
	
	emit changed(false);
}
 
void KeyboardConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Keyboard"));
	save();
}

KeyboardConfiguration::~KeyboardConfiguration()
{
	delete setContainer;
}
