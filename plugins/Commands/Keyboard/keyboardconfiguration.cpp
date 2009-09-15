/*
 *   Copyright (C) 2009 Dominik Neumeister & Mario Strametz <neudob06@edvhtl.at>  <strmam06@htl-kaindorf.ac.at>
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

KeyboardConfiguration::KeyboardConfiguration(KeyboardCommandManager* _commandManager, QWidget *parent, const QVariantList &args)
		: CommandConfiguration("keyboard", ki18n( "Keyboard" ), 
				      "0.1", ki18n("Input signes with ease"),
				      "input-keyboard",
				      KeyboardCommandPluginFactory::componentData(),parent),
				      storedSet(NULL),
				      commandManager(_commandManager),
				      setContainer(_commandManager->getKeyboardSetContainer())
{
	kDebug() << "Constructor";
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(KeyboardCommandPluginFactory::componentData(),"simonkeyboardrc");

	QObject::connect(ui.cbShowNumpad, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.cbCaseSensitivity, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
        connect(ui.pbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
	connect(ui.pbDeleteSet, SIGNAL(clicked()), this, SLOT(deleteSet()));
	connect(ui.pbAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
	connect(ui.pbDeleteTab, SIGNAL(clicked()), this, SLOT(deleteTab()));
        connect(ui.pbAddButton, SIGNAL(clicked()), this, SLOT(addButton()));
	connect(ui.pbDeleteButton, SIGNAL(clicked()), this, SLOT(deleteButton()));
	connect(ui.pbUpButton, SIGNAL(clicked()), this, SLOT(buttonUp()));
	connect(ui.pbDownButton, SIGNAL(clicked()), this, SLOT(buttonDown()));
        connect(ui.cbSets, SIGNAL(currentIndexChanged(int)), this, SLOT(cbSetsIndexChanged()));
        connect(ui.cbSets, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
        connect(ui.cbTabs, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTabDetail()));
	connect(ui.pbTabUp, SIGNAL(clicked()), this, SLOT(tabUp()));
	connect(ui.pbTabDown, SIGNAL(clicked()), this, SLOT(tabDown()));
	connect(ui.cbShowNumpad, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbEnableNumberBasedSelection, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbEnableNumberWriteOut, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbShift, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbCapsLock, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbControl, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbBackspace, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbReturn, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbAlt, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbAltGr, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbSuper, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.leNumberBackspaceTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leNumberBasedSelectionTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leNumberWriteOutTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leShiftTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leCapsLockTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leControlTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leBackspaceTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leReturnTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leAltTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leAltGrTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	connect(ui.leSuperTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));


	ui.pbAddSet->setIcon(KIcon("list-add"));
	ui.pbAddTab->setIcon(KIcon("list-add"));
	ui.pbAddButton->setIcon(KIcon("list-add"));

	ui.pbDeleteSet->setIcon(KIcon("list-remove"));
	ui.pbDeleteTab->setIcon(KIcon("list-remove"));
	ui.pbDeleteButton->setIcon(KIcon("list-remove"));

	ui.pbTabUp->setIcon(KIcon("arrow-up"));
	ui.pbUpButton->setIcon(KIcon("arrow-up"));
	ui.pbTabDown->setIcon(KIcon("arrow-down"));
	ui.pbDownButton->setIcon(KIcon("arrow-down"));
}

bool KeyboardConfiguration::showNumpad()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("NumPad", false);
}

bool KeyboardConfiguration::caseSensitive()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("CaseSensitivity", false);
}

bool KeyboardConfiguration::enableNumberBasedSelection()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("EnableNumberBasedSelection", false);
}

QString KeyboardConfiguration::numberBasedSelectionTrigger()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("NumberBasedSelectionTrigger", i18n("Select number"));
}

bool KeyboardConfiguration::enableNumberWriteOut()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("EnableNumberWriteOut", true);
}


QString KeyboardConfiguration::numberBackspaceTrigger()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("NumberBackspace", i18n("Number backspace"));
}

QString KeyboardConfiguration::numberWriteOutTrigger()
{
	KConfigGroup cg(config, "NumPad");
	return cg.readEntry("NumberWriteOutTrigger", i18n("Write out number"));
}

bool KeyboardConfiguration::capsLock()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("CapsLock", false);
}

bool KeyboardConfiguration::shift()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Shift", true);
}

QString KeyboardConfiguration::shiftTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("ShiftTrigger", i18n("Shift"));
}

QString KeyboardConfiguration::capsLockTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("CapsLockTrigger", i18n("Caps lock"));
}

bool KeyboardConfiguration::backspace()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Backspace", true);
}

bool KeyboardConfiguration::returnKey()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Return", true);
}

QString KeyboardConfiguration::returnKeyTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("ReturnTrigger", i18n("Return"));
}

QString KeyboardConfiguration::backspaceTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("BackspaceTrigger", i18n("Backspace"));
}

bool KeyboardConfiguration::control()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Control", true);
}

QString KeyboardConfiguration::controlTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("ControlTrigger", i18n("Control"));
}

bool KeyboardConfiguration::alt()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Alt", false);
}

QString KeyboardConfiguration::altTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("AltTrigger", i18n("Alternate"));
}

bool KeyboardConfiguration::altGr()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("AltGr", false);
}

QString KeyboardConfiguration::altGrTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("AltGrTrigger", i18n("Alternate Graphics"));
}

bool KeyboardConfiguration::super()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("Super", false);
}

QString KeyboardConfiguration::superTrigger()
{
	KConfigGroup cg(config, "SpecialKeys");
	return cg.readEntry("SuperTrigger", i18n("Super"));
}

QPoint KeyboardConfiguration::keyboardPosition()
{
	KConfigGroup cg(config, "KeyboardGeometry");
	return cg.readEntry("Position", QPoint());
}

QSize KeyboardConfiguration::keyboardSize()
{
	KConfigGroup cg(config, "KeyboardGeometry");
	return cg.readEntry("Size", QSize());
}

void KeyboardConfiguration::saveKeyboardGeometry(const QPoint& position, const QSize& size)
{
	KConfigGroup cg(config, "KeyboardGeometry");
	cg.writeEntry("Position", position);
	cg.writeEntry("Size", size);
	cg.sync();
}

void KeyboardConfiguration::addSet()
{	
        bool ok;
        QString inputText = QInputDialog::getText(this, "Add keyboard set", "Please enter the name of the new set:", QLineEdit::Normal, QString(), &ok);
	if (!inputText.isEmpty()) {
		if (!setContainer->createSet(inputText))
			KMessageBox::sorry(this, i18n("Failed to add set"));
		refreshCbSets();
		//switch to new set
		ui.cbSets->setCurrentIndex(ui.cbSets->count()-1);
		refreshCbTabs();
		emit changed(true);
	}
}
void KeyboardConfiguration::deleteSet()
{	
        if(ui.cbSets->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please choose a set to be deleted"));
		return;
	}
	if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to delete the selected set?")) != KMessageBox::Yes)
		return;
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
	//switch to new tab
	ui.cbTabs->setCurrentIndex(ui.cbTabs->count()-1);
	refreshTabDetail();
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

	if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to delete the selected tab?")) != KMessageBox::Yes)
		return;
	if (!setContainer->deleteTab(ui.cbSets->currentText(), tab))
		KMessageBox::sorry(this, i18n("Failed to delete tab"));

	refreshCbTabs();
	emit changed(true);
}

void KeyboardConfiguration::addButton()
{
	if(ui.cbSets->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please select a set to which to add the new button"));
		return;
	}
	if(ui.cbTabs->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please select a tab to which to add the new button"));
		return;
	}
	KeyboardAddButtonDialog *kab = new KeyboardAddButtonDialog(this);
	KeyboardButton *kbb =  kab->addButton();
	if(kbb!=NULL)
	{
		if (!setContainer->addButton(ui.cbSets->currentText(),
						ui.cbTabs->currentText(), kbb))
			KMessageBox::sorry(this, i18n("Failed to add button"));

		emit changed(true);
	}
}

void KeyboardConfiguration::deleteButton()
{
	if(ui.cbSets->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please select a set to which to add the new button"));
		return;
	}
	if(ui.cbTabs->currentIndex() == -1) {
		KMessageBox::information(this, i18n("Please select a tab to which to add the new button"));
		return;
	}

	KeyboardButton *button = static_cast<KeyboardButton*>(ui.tvTabContent->currentIndex().internalPointer());
	if (!button)  {
		KMessageBox::information(this, i18n("Please select a button to delete from the list"));
		return;
	}

	if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to delete the selected button?")) != KMessageBox::Yes)
		return;

	if (!setContainer->deleteButton(ui.cbSets->currentText(), ui.cbTabs->currentText(), button))
			KMessageBox::sorry(this, i18n("Failed to delete button"));

	emit changed(true);

}

void KeyboardConfiguration::tabUp()
{
	QString tab = ui.cbTabs->currentText();
	if (tab.isEmpty()) {
		KMessageBox::information(this, i18n("Please select the tab to be moved"));
		return;
	}


	if (!setContainer->moveTabUp(ui.cbSets->currentText(), tab)) {
		KMessageBox::sorry(this, i18n("Failed to move tab up. Maybe it is already at the top?"));
		return;
	}

	int cur = ui.cbTabs->currentIndex();
	refreshCbTabs();
	ui.cbTabs->setCurrentIndex(cur-1);
	refreshTabDetail();
	emit changed(true);
}

void KeyboardConfiguration::tabDown()
{
	QString tab = ui.cbTabs->currentText();
	if (tab.isEmpty()) {
		KMessageBox::information(this, i18n("Please select the tab to be moved"));
		return;
	}

	if (!setContainer->moveTabDown(ui.cbSets->currentText(), tab)) {
		KMessageBox::sorry(this, i18n("Failed to move tab down. Maybe it is already at the bottom?"));
		return;
	}

	int cur = ui.cbTabs->currentIndex();
	refreshCbTabs();
	ui.cbTabs->setCurrentIndex(cur+1);
	refreshTabDetail();
	emit changed(true);
}


void KeyboardConfiguration::buttonUp()
{
	KeyboardButton *button = static_cast<KeyboardButton*>(ui.tvTabContent->currentIndex().internalPointer());
	if (!button)  {
		KMessageBox::information(this, i18n("Please select a button to delete from the list"));
		return;
	}

	int row = ui.tvTabContent->currentIndex().row();
	if (!setContainer->moveButtonUp(ui.cbSets->currentText(), ui.cbTabs->currentText(), button)) {
		KMessageBox::sorry(this, i18n("Button could not be moved up. Is it already at the top?"));
	} else {
		ui.tvTabContent->selectRow(row-1);
		emit changed(true);
	}
}

void KeyboardConfiguration::buttonDown()
{
	KeyboardButton *button = static_cast<KeyboardButton*>(ui.tvTabContent->currentIndex().internalPointer());
	if (!button)  {
		KMessageBox::information(this, i18n("Please select a button to delete from the list"));
		return;
	}

	int row = ui.tvTabContent->currentIndex().row();
	if (!setContainer->moveButtonDown(ui.cbSets->currentText(), ui.cbTabs->currentText(), button)) {
		KMessageBox::sorry(this, i18n("Button could not be moved down. Is it already at the bottom?"));
	} else {
		ui.tvTabContent->selectRow(row+1);
		emit changed(true);
	}
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
	kDebug() << "Save";
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("CaseSensitivity", ui.cbCaseSensitivity->isChecked());
	cg.writeEntry("SelectedSet", ui.cbSets->currentText());
        cg.sync();

	KConfigGroup cgNumPad(config, "NumPad");
	cgNumPad.writeEntry("NumPad", ui.cbShowNumpad->isChecked());
	cgNumPad.writeEntry("NumberBackspace", ui.leNumberBackspaceTrigger->text());
	cgNumPad.writeEntry("EnableNumberBasedSelection", ui.cbEnableNumberBasedSelection->isChecked());
	cgNumPad.writeEntry("NumberBasedSelectionTrigger", ui.leNumberBasedSelectionTrigger->text());
	cgNumPad.writeEntry("EnableNumberWriteOut", ui.cbEnableNumberWriteOut->isChecked());
	cgNumPad.writeEntry("NumberWriteOutTrigger", ui.leNumberWriteOutTrigger->text());
	cgNumPad.sync();

	KConfigGroup cgSpecialKeys(config, "SpecialKeys");
	cgSpecialKeys.writeEntry("Shift", ui.cbShift->isChecked());
	cgSpecialKeys.writeEntry("ShiftTrigger", ui.leShiftTrigger->text());
	cgSpecialKeys.writeEntry("Backspace", ui.cbBackspace->isChecked());
	cgSpecialKeys.writeEntry("BackspaceTrigger", ui.leBackspaceTrigger->text());
	cgSpecialKeys.writeEntry("Control", ui.cbControl->isChecked());
	cgSpecialKeys.writeEntry("ControlTrigger", ui.leControlTrigger->text());
	cgSpecialKeys.writeEntry("CapsLock", ui.cbCapsLock->isChecked());
	cgSpecialKeys.writeEntry("CapsLockTrigger", ui.leCapsLockTrigger->text());
	cgSpecialKeys.writeEntry("Return", ui.cbReturn->isChecked());
	cgSpecialKeys.writeEntry("ReturnTrigger", ui.leReturnTrigger->text());

	cgSpecialKeys.writeEntry("Alt", ui.cbAlt->isChecked());
	cgSpecialKeys.writeEntry("AltTrigger", ui.leAltTrigger->text());
	cgSpecialKeys.writeEntry("AltGr", ui.cbAltGr->isChecked());
	cgSpecialKeys.writeEntry("AltGrTrigger", ui.leAltGrTrigger->text());
	cgSpecialKeys.writeEntry("Super", ui.cbSuper->isChecked());
	cgSpecialKeys.writeEntry("SuperTrigger", ui.leSuperTrigger->text());
	cgSpecialKeys.sync();

	if (!setContainer->save()) {
		KMessageBox::sorry(this, i18n("Failed to save keyboard sets"));
		return;
	}

	if (!ui.cbSets->currentText().isEmpty()) { KeyboardSet *s = setContainer->findSet(ui.cbSets->currentText());
		storedSet = s;
	}
	commandManager->rebuildGui();

	emit changed(false);
}

void KeyboardConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void KeyboardConfiguration::load()
{
	kDebug() << "Load";
        Q_ASSERT(config);
	KConfigGroup cg(config, "");
	ui.cbCaseSensitivity->setChecked(cg.readEntry("CaseSensitivity", false));

	KConfigGroup cgNumPad(config, "NumPad");
	ui.cbShowNumpad->setChecked(cgNumPad.readEntry("NumPad", false));
	ui.leNumberBackspaceTrigger->setText(cgNumPad.readEntry("NumberBackspace", i18n("Number Backspace")));
	ui.cbEnableNumberBasedSelection->setChecked(cgNumPad.readEntry("EnableNumberBasedSelection", false));
	ui.leNumberBasedSelectionTrigger->setText(cgNumPad.readEntry("NumberBasedSelectionTrigger", i18n("Select number")));
	ui.cbEnableNumberWriteOut->setChecked(cg.readEntry("EnableNumberWriteOut", true));
	ui.leNumberWriteOutTrigger->setText(cgNumPad.readEntry("NumberWriteOutTrigger", i18n("Write number")));

	KConfigGroup cgSpecialKeys(config, "SpecialKeys");
	ui.cbShift->setChecked(cgSpecialKeys.readEntry("Shift", true));
	ui.leShiftTrigger->setText(cgSpecialKeys.readEntry("ShiftTrigger", i18n("Shift")));
	ui.cbBackspace->setChecked(cgSpecialKeys.readEntry("Backspace", true));
	ui.leBackspaceTrigger->setText(cgSpecialKeys.readEntry("BackspaceTrigger", i18n("Backspace")));
	ui.cbCapsLock->setChecked(cgSpecialKeys.readEntry("CapsLock", false));
	ui.leCapsLockTrigger->setText(cgSpecialKeys.readEntry("CapsLockTrigger", i18n("CapsLock")));
	ui.cbControl->setChecked(cgSpecialKeys.readEntry("Control", true));
	ui.leControlTrigger->setText(cgSpecialKeys.readEntry("ControlTrigger", i18n("Control")));
	ui.cbReturn->setChecked(cgSpecialKeys.readEntry("Return", true));
	ui.leReturnTrigger->setText(cgSpecialKeys.readEntry("ReturnTrigger", i18n("Return")));
	ui.cbAlt->setChecked(cgSpecialKeys.readEntry("Alt", false));
	ui.leAltTrigger->setText(cgSpecialKeys.readEntry("AltTrigger", i18n("Alternate")));
	ui.cbAltGr->setChecked(cgSpecialKeys.readEntry("AltGr", false));
	ui.leAltGrTrigger->setText(cgSpecialKeys.readEntry("AltGrTrigger", i18n("Alternate Graphic")));
	ui.cbSuper->setChecked(cgSpecialKeys.readEntry("Super", false));
	ui.leSuperTrigger->setText(cgSpecialKeys.readEntry("SuperTrigger", i18n("Super")));

	QString selectedSet = cg.readEntry("SelectedSet", "Basic");
	cg.sync();

	setContainer->clear();
	if (!setContainer->load()) {
		KMessageBox::sorry(this, i18n("Failed to load keyboard sets from storage"));
		return;
	}

	refreshCbSets();


	int index = ui.cbSets->findText(selectedSet);
	if (index != -1) {
		ui.cbSets->setCurrentIndex(index);
		refreshCbTabs();
	}
	
	KeyboardSet *newStoredSet = setContainer->findSet(selectedSet);
	
	storedSet = newStoredSet;
	commandManager->rebuildGui();

	emit changed(false);
}
 
void KeyboardConfiguration::defaults()
{
	ui.cbCaseSensitivity->setChecked(false);
	ui.cbShowNumpad->setChecked(false);

	ui.leNumberBackspaceTrigger->setText(i18n("Number Backspace"));
	ui.cbEnableNumberBasedSelection->setChecked(false);
	ui.leNumberBasedSelectionTrigger->setText( i18n("Select number"));
	ui.cbEnableNumberWriteOut->setChecked(true);
	ui.leNumberWriteOutTrigger->setText(i18n("Write number"));

	ui.cbShift->setChecked(true);
	ui.leShiftTrigger->setText(i18n("Shift"));
	ui.cbBackspace->setChecked(true);
	ui.leBackspaceTrigger->setText(i18n("Backspace"));
	ui.cbCapsLock->setChecked(false);
	ui.leCapsLockTrigger->setText(i18n("CapsLock"));
	ui.cbControl->setChecked(true);
	ui.leControlTrigger->setText(i18n("Control"));
	ui.cbReturn->setChecked(true);
	ui.leReturnTrigger->setText(i18n("Return"));
	ui.cbAlt->setChecked(false);
	ui.leAltTrigger->setText(i18n("Alternate"));
	ui.cbAltGr->setChecked(false);
	ui.leAltGrTrigger->setText(i18n("Alternate Graphic"));
	ui.cbSuper->setChecked(false);
	ui.leSuperTrigger->setText(i18n("Super"));

	save();
}

KeyboardConfiguration::~KeyboardConfiguration()
{
	//don't delete my precious!
	ui.tvTabContent->setModel(NULL);

	kDebug() << "Deleting keyboardconfiguration";
	//setContainer managed by manager
}
