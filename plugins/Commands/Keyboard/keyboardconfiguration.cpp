/* *   Copyright (C) 2009 Dominik Neumeister & Mario Strametz <neudob06@edvhtl.at>  <strmam06@htl-kaindorf.ac.at>
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
#include "keyboardmodifybuttondialog.h"

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
#include <simonscenarios/scenario.h>


K_PLUGIN_FACTORY_DECLARATION(KeyboardCommandPluginFactory)


KeyboardConfiguration::KeyboardConfiguration(KeyboardCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
		: CommandConfiguration(parent,  "keyboard", ki18n( "Keyboard" ), 
				      "0.1", ki18n("Input signes with ease"),
				      "input-keyboard",
				      KeyboardCommandPluginFactory::componentData()),
				      storedSet(NULL),
				      commandManager(_commandManager),
				      setContainer(_commandManager->getKeyboardSetContainer())
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	QObject::connect(ui.cbShowNumpad, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.cbCaseSensitivity, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
        connect(ui.pbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
	connect(ui.pbEditSet, SIGNAL(clicked()), this, SLOT(editSet()));
	connect(ui.pbDeleteSet, SIGNAL(clicked()), this, SLOT(deleteSet()));
	connect(ui.pbAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
	connect(ui.pbEditTab, SIGNAL(clicked()), this, SLOT(editTab()));
	connect(ui.pbDeleteTab, SIGNAL(clicked()), this, SLOT(deleteTab()));
        connect(ui.pbAddButton, SIGNAL(clicked()), this, SLOT(addButton()));
	connect(ui.pbEditButton, SIGNAL(clicked()), this, SLOT(editButton()));
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

	ui.pbEditSet->setIcon(KIcon("document-edit"));
	ui.pbEditTab->setIcon(KIcon("document-edit"));
	ui.pbEditButton->setIcon(KIcon("document-edit"));

	ui.pbTabUp->setIcon(KIcon("arrow-up"));
	ui.pbUpButton->setIcon(KIcon("arrow-up"));
	ui.pbTabDown->setIcon(KIcon("arrow-down"));
	ui.pbDownButton->setIcon(KIcon("arrow-down"));
}

bool KeyboardConfiguration::showNumpad()
{
	return ui.cbShowNumpad->isChecked();
}

bool KeyboardConfiguration::caseSensitive()
{
	return ui.cbCaseSensitivity->isChecked();
}

bool KeyboardConfiguration::enableNumberBasedSelection()
{
	return ui.cbEnableNumberBasedSelection->isChecked();
}

QString KeyboardConfiguration::numberBasedSelectionTrigger()
{
	return ui.leNumberBasedSelectionTrigger->text();
}

bool KeyboardConfiguration::enableNumberWriteOut()
{
	return ui.cbEnableNumberWriteOut->isChecked();
}

QString KeyboardConfiguration::numberBackspaceTrigger()
{
	return ui.leNumberBackspaceTrigger->text();
}

QString KeyboardConfiguration::numberWriteOutTrigger()
{
	return ui.leNumberWriteOutTrigger->text();
}

bool KeyboardConfiguration::capsLock()
{
	return ui.cbCapsLock->isChecked();
}

bool KeyboardConfiguration::shift()
{
	return ui.cbShift->isChecked();
}

QString KeyboardConfiguration::shiftTrigger()
{
	return ui.leShiftTrigger->text();
}

QString KeyboardConfiguration::capsLockTrigger()
{
	return ui.leCapsLockTrigger->text();
}

bool KeyboardConfiguration::backspace()
{
	return ui.cbBackspace->isChecked();
}

bool KeyboardConfiguration::returnKey()
{
	return ui.cbReturn->isChecked();
}

QString KeyboardConfiguration::returnKeyTrigger()
{
	return ui.leReturnTrigger->text();
}

QString KeyboardConfiguration::backspaceTrigger()
{
	return ui.leBackspaceTrigger->text();
}

bool KeyboardConfiguration::control()
{
	return ui.cbControl->isChecked();
}

QString KeyboardConfiguration::controlTrigger()
{
	return ui.leControlTrigger->text();
}

bool KeyboardConfiguration::alt()
{
	return ui.cbAlt->isChecked();
}

QString KeyboardConfiguration::altTrigger()
{
	return ui.leAltTrigger->text();
}

bool KeyboardConfiguration::altGr()
{
	return ui.cbAltGr->isChecked();
}

QString KeyboardConfiguration::altGrTrigger()
{
	return ui.leAltGrTrigger->text();
}

bool KeyboardConfiguration::super()
{
	return ui.cbSuper->isChecked();
}

QString KeyboardConfiguration::superTrigger()
{
	return ui.leSuperTrigger->text();
}

QPoint KeyboardConfiguration::keyboardPosition()
{
	return m_keyboardPosition;
}

QSize KeyboardConfiguration::keyboardSize()
{
	return m_keyboardSize;
}

void KeyboardConfiguration::saveKeyboardGeometry(const QPoint& position, const QSize& size)
{
	m_keyboardSize = size;
	m_keyboardPosition = position;

	parentScenario->save();
}

void KeyboardConfiguration::addSet()
{	
        bool ok;
        QString inputText = QInputDialog::getText(this, "Add keyboard set", "Please enter the name of the new set:", QLineEdit::Normal, QString(), &ok);
	if (ok && !inputText.isEmpty()) {
		if (!setContainer->createSet(inputText))
			KMessageBox::sorry(this, i18n("Failed to add set"));
		refreshCbSets();
		//switch to new set
		ui.cbSets->setCurrentIndex(ui.cbSets->count()-1);
		refreshCbTabs();
		emit changed(true);
	}
}


void KeyboardConfiguration::editSet()
{
	int currentIndex = ui.cbSets->currentIndex();
	if(currentIndex == -1) {
		KMessageBox::information(this, i18n("Please choose a set to be edited"));
		return;
	}

	QString oldSetName = ui.cbSets->currentText();
        bool ok;
        QString inputText = QInputDialog::getText(this, "Edit keyboard set", "Please enter the new name of the set:", QLineEdit::Normal, oldSetName, &ok);
	if (ok && !inputText.isEmpty() && (oldSetName != inputText)) {
		if (!setContainer->editSet(oldSetName, inputText))
			KMessageBox::sorry(this, i18n("Failed to edit set"));
		refreshCbSets();
		ui.cbSets->setCurrentIndex(currentIndex);
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

void KeyboardConfiguration::editTab()
{	
	if(ui.cbSets->currentIndex() == -1)  {
		KMessageBox::information(this, i18n("Please insert or select a set first"));
		return;
	}
	QString oldName = ui.cbTabs->currentText();
	int currentIndex = ui.cbTabs->currentIndex();
	if (oldName.isEmpty()) {
		KMessageBox::information(this, i18n("Please select the tab to be edited"));
		return;
	}

	QString inputText = QInputDialog::getText(this, "Edit keyboard tab", "Please enter the new name of the tab:", QLineEdit::Normal, oldName);

	if(!inputText.isEmpty()) {
		if (!setContainer->editTab(ui.cbSets->currentText(), oldName, inputText))
			KMessageBox::sorry(this, i18n("Failed to edit tab"));
	}

	refreshCbTabs();
	ui.cbTabs->setCurrentIndex(currentIndex);
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
	KeyboardModifyButtonDialog *modifyDialog = new KeyboardModifyButtonDialog(this);
	KeyboardButton *kbb = modifyDialog->addButton();
	if(kbb!=NULL)
	{
		if (setContainer->addButton(ui.cbSets->currentText(),
						ui.cbTabs->currentText(), kbb))
			emit changed(true);
	}
	modifyDialog->deleteLater();
}


void KeyboardConfiguration::editButton()
{
	//TODO
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

	KeyboardModifyButtonDialog *modifyDialog = new KeyboardModifyButtonDialog(this);
	
	if (!modifyDialog->editButton(button))
		KMessageBox::sorry(this, i18n("Failed to edit button"));
	else
		emit changed(true);
	
	modifyDialog->deleteLater();
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


QDomElement KeyboardConfiguration::serialize(QDomDocument* doc)
{
	kDebug() << "Serialize is called...";

	QDomElement configElem = doc->createElement("config");

	//general
	QDomElement caseSensitivityElem = doc->createElement("caseSensitivity");
	caseSensitivityElem.appendChild(doc->createTextNode(ui.cbCaseSensitivity->isChecked() ? "1" : "0"));
	configElem.appendChild(caseSensitivityElem);

	QDomElement selectedSetElem = doc->createElement("selectedSet");
	selectedSetElem.appendChild(doc->createTextNode(ui.cbSets->currentText()));
	configElem.appendChild(selectedSetElem);

	//numpad
	QDomElement numpadElem= doc->createElement("numpad");

	QDomElement showNumPadElem = doc->createElement("showNumpad");
	showNumPadElem.appendChild(doc->createTextNode(ui.cbShowNumpad->isChecked() ? "1" : "0"));
	numpadElem.appendChild(showNumPadElem);

	QDomElement numberBackspaceElem = doc->createElement("numberBackspace");
	numberBackspaceElem.appendChild(doc->createTextNode(ui.leNumberBackspaceTrigger->text()));
	numpadElem.appendChild(numberBackspaceElem);

	QDomElement enableNumberBasedSelectionElem = doc->createElement("enableNumberBasedSelection");
	enableNumberBasedSelectionElem.appendChild(doc->createTextNode(ui.cbEnableNumberBasedSelection->isChecked() ? "1" : "0"));
	numpadElem.appendChild(enableNumberBasedSelectionElem);

	QDomElement numberBasedSelectionTriggerElem = doc->createElement("numberBasedSelectionTrigger");
	numberBasedSelectionTriggerElem.appendChild(doc->createTextNode(ui.leNumberBasedSelectionTrigger->text()));
	numpadElem.appendChild(numberBasedSelectionTriggerElem);

	QDomElement enableNumberWriteOutElem = doc->createElement("enableNumberWriteOut");
	enableNumberWriteOutElem.appendChild(doc->createTextNode(ui.cbEnableNumberWriteOut->isChecked() ? "1" : "0"));
	numpadElem.appendChild(enableNumberWriteOutElem);

	QDomElement numberWriteOutTriggerElem = doc->createElement("numberWriteOutTrigger");
	numberWriteOutTriggerElem.appendChild(doc->createTextNode(ui.leNumberBasedSelectionTrigger->text()));
	numpadElem.appendChild(numberWriteOutTriggerElem);

	configElem.appendChild(numpadElem);
	

	//special keys
	QDomElement specialKeysElem= doc->createElement("specialKeys");

	QDomElement shiftElem = doc->createElement("shift");
	shiftElem.appendChild(doc->createTextNode(ui.cbShift->isChecked() ? "1" : "0"));
	QDomElement shiftTriggerElem = doc->createElement("shiftTrigger");
	shiftTriggerElem.appendChild(doc->createTextNode(ui.leShiftTrigger->text()));
	specialKeysElem.appendChild(shiftElem);
	specialKeysElem.appendChild(shiftTriggerElem);

	QDomElement backspaceElem = doc->createElement("backspace");
	backspaceElem.appendChild(doc->createTextNode(ui.cbBackspace->isChecked() ? "1" : "0"));
	QDomElement backspaceTriggerElem = doc->createElement("backspaceTrigger");
	backspaceTriggerElem.appendChild(doc->createTextNode(ui.leBackspaceTrigger->text()));
	specialKeysElem.appendChild(backspaceElem);
	specialKeysElem.appendChild(backspaceTriggerElem);

	QDomElement controlElem = doc->createElement("control");
	controlElem.appendChild(doc->createTextNode(ui.cbControl->isChecked() ? "1" : "0"));
	QDomElement controlTriggerElem = doc->createElement("controlTrigger");
	controlTriggerElem.appendChild(doc->createTextNode(ui.leControlTrigger->text()));
	specialKeysElem.appendChild(controlElem);
	specialKeysElem.appendChild(controlTriggerElem);

	QDomElement capsLockElem = doc->createElement("capsLock");
	capsLockElem.appendChild(doc->createTextNode(ui.cbCapsLock->isChecked() ? "1" : "0"));
	QDomElement capsLockTriggerElem = doc->createElement("capsLockTrigger");
	capsLockTriggerElem.appendChild(doc->createTextNode(ui.leCapsLockTrigger->text()));
	specialKeysElem.appendChild(capsLockElem);
	specialKeysElem.appendChild(capsLockTriggerElem);

	QDomElement returnElem = doc->createElement("return");
	returnElem.appendChild(doc->createTextNode(ui.cbReturn->isChecked() ? "1" : "0"));
	QDomElement returnTriggerElem = doc->createElement("returnTrigger");
	returnTriggerElem.appendChild(doc->createTextNode(ui.leReturnTrigger->text()));
	specialKeysElem.appendChild(returnElem);
	specialKeysElem.appendChild(returnTriggerElem);

	QDomElement altElem = doc->createElement("alt");
	altElem.appendChild(doc->createTextNode(ui.cbAlt->isChecked() ? "1" : "0"));
	QDomElement altTriggerElem = doc->createElement("altTrigger");
	altTriggerElem.appendChild(doc->createTextNode(ui.leAltTrigger->text()));
	specialKeysElem.appendChild(altElem);
	specialKeysElem.appendChild(altTriggerElem);

	QDomElement altGrElem = doc->createElement("altGr");
	altGrElem.appendChild(doc->createTextNode(ui.cbAltGr->isChecked() ? "1" : "0"));
	QDomElement altGrTriggerElem = doc->createElement("altGrTrigger");
	altGrTriggerElem.appendChild(doc->createTextNode(ui.leAltGrTrigger->text()));
	specialKeysElem.appendChild(altGrElem);
	specialKeysElem.appendChild(altGrTriggerElem);
	
	QDomElement superElem = doc->createElement("super");
	superElem.appendChild(doc->createTextNode(ui.cbSuper->isChecked() ? "1" : "0"));
	QDomElement superTriggerElem = doc->createElement("superTrigger");
	superTriggerElem.appendChild(doc->createTextNode(ui.leSuperTrigger->text()));
	specialKeysElem.appendChild(superElem);
	specialKeysElem.appendChild(superTriggerElem);
	
	configElem.appendChild(specialKeysElem);
	
	//sets
	QDomElement setsElem = setContainer->serialize(doc);
	configElem.appendChild(setsElem);

	if (!ui.cbSets->currentText().isEmpty()) 
		storedSet = setContainer->findSet(ui.cbSets->currentText());
	
	kDebug() << "Calling rebuild gui";
	commandManager->rebuildGui();

	return configElem;
}

bool KeyboardConfiguration::deSerialize(const QDomElement& elem)
{
	setContainer->clear();
	QDomElement setsElement = elem.firstChildElement("keyboardSets");
	if (!setContainer->deSerialize(setsElement)) {
		KMessageBox::sorry(this, i18n("Failed to load keyboard sets from storage"));
		return false;
	}

	refreshCbSets();
	QDomElement caseSensitivityElem = elem.firstChildElement("caseSensitivity");
	if (caseSensitivityElem.isNull()) {
		defaults();
		return true;
	}
	ui.cbCaseSensitivity->setChecked(caseSensitivityElem.text() == "1");

	QString selectedSet = elem.firstChildElement("selectedSet").text();
	
	int index = ui.cbSets->findText(selectedSet);
	if (index != -1) {
		ui.cbSets->setCurrentIndex(index);
		refreshCbTabs();
	}
	KeyboardSet *newStoredSet = setContainer->findSet(selectedSet);
	storedSet = newStoredSet;

	//numpad
	QDomElement numpadElement = elem.firstChildElement("numpad");
	ui.cbShowNumpad->setChecked(numpadElement.firstChildElement("showNumpad").text() == "1");
	
	ui.leNumberBackspaceTrigger->setText(numpadElement.firstChildElement("numberBackspace").text());

	ui.cbEnableNumberBasedSelection->setChecked(numpadElement.firstChildElement("enableNumberBasedSelection").text() == "1");
	ui.leNumberBasedSelectionTrigger->setText(numpadElement.firstChildElement("numberBasedSelectionTrigger").text());

	ui.cbEnableNumberWriteOut->setChecked(numpadElement.firstChildElement("enableNumberWriteOut").text() == "1");
	ui.leNumberWriteOutTrigger->setText(numpadElement.firstChildElement("numberWriteOutTrigger").text());

	//special keys
	QDomElement specialKeysElement = elem.firstChildElement("specialKeys");
	ui.cbShift->setChecked(specialKeysElement.firstChildElement("shift").text() == "1");
	ui.leShiftTrigger->setText(specialKeysElement.firstChildElement("shiftTrigger").text());
	ui.cbBackspace->setChecked(specialKeysElement.firstChildElement("backspace").text() == "1");
	ui.leBackspaceTrigger->setText(specialKeysElement.firstChildElement("backspaceTrigger").text());
	ui.cbCapsLock->setChecked(specialKeysElement.firstChildElement("capsLock").text() == "1");
	ui.leCapsLockTrigger->setText(specialKeysElement.firstChildElement("capsLockTrigger").text());
	ui.cbControl->setChecked(specialKeysElement.firstChildElement("control").text() == "1");
	ui.leControlTrigger->setText(specialKeysElement.firstChildElement("controlTrigger").text());
	ui.cbReturn->setChecked(specialKeysElement.firstChildElement("return").text() == "1");
	ui.leReturnTrigger->setText(specialKeysElement.firstChildElement("returnTrigger").text());
	ui.cbAlt->setChecked(specialKeysElement.firstChildElement("alt").text() == "1");
	ui.leAltTrigger->setText(specialKeysElement.firstChildElement("altTrigger").text());
	ui.cbAltGr->setChecked(specialKeysElement.firstChildElement("altGr").text() == "1");
	ui.leAltGrTrigger->setText(specialKeysElement.firstChildElement("altGrTrigger").text());
	ui.cbSuper->setChecked(specialKeysElement.firstChildElement("super").text() == "1");
	ui.leSuperTrigger->setText(specialKeysElement.firstChildElement("superTrigger").text());

	commandManager->rebuildGui();
	return true;
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

	QString selectedSet = "Basic";

	int index = ui.cbSets->findText(selectedSet);
	if (index != -1) {
		ui.cbSets->setCurrentIndex(index);
		refreshCbTabs();
	}
}

KeyboardConfiguration::~KeyboardConfiguration()
{
	//don't delete my precious!
	ui.tvTabContent->setModel(NULL);
}

