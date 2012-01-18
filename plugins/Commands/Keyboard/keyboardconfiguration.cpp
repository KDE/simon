/*   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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
#include "keyboardmodifybuttondialog.h"

#include <QVariantList>
#include <kgenericfactory.h>
#include <QList>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>
#include <QString>
#include <QDialog>
#include <KInputDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>
#include <simonscenarios/scenario.h>

K_PLUGIN_FACTORY_DECLARATION(KeyboardCommandPluginFactory)

KeyboardConfiguration::KeyboardConfiguration(KeyboardCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "keyboard", ki18n( "Keyboard" ),
"0.1", ki18n("Input signs with ease"),
"input-keyboard",
KeyboardCommandPluginFactory::componentData()),
storedSet(0),
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
  QString inputText = KInputDialog::getText(i18n("Add keyboard set"), i18n("Please enter the name of the new set:"), QString(), &ok);
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
  QString inputText = KInputDialog::getText(i18n("Edit keyboard set"), i18n("Please enter the new name of the set:"), oldSetName, &ok);
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
    KMessageBox::sorry(this, i18n("Could not delete set"));

  refreshCbSets();
  emit changed(true);                             //when the delete failed we should still write the internal structure
}


void KeyboardConfiguration::addTab()
{
  if(ui.cbSets->currentIndex() == -1) {
    KMessageBox::information(this, i18n("Please insert or select a set first"));
    return;
  }

  QString inputText = KInputDialog::getText(i18n("Add keyboard tab"),i18n("Please enter the name of the new tab:"));

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
  if(ui.cbSets->currentIndex() == -1) {
    KMessageBox::information(this, i18n("Please insert or select a set first"));
    return;
  }
  QString oldName = ui.cbTabs->currentText();
  int currentIndex = ui.cbTabs->currentIndex();
  if (oldName.isEmpty()) {
    KMessageBox::information(this, i18n("Please select the tab to be edited"));
    return;
  }

  QString inputText = KInputDialog::getText(i18n("Edit keyboard tab"), i18n("Please enter the new name of the tab:"), oldName);

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
  if(ui.cbSets->currentIndex() == -1) {
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
  if(kbb!=0) {
    if (setContainer->addButton(ui.cbSets->currentText(),
      ui.cbTabs->currentText(), kbb))
      emit changed(true);
  }
  modifyDialog->deleteLater();
}


void KeyboardConfiguration::editButton()
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
  if (!button) {
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
  if (!button) {
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
  if (!button) {
    KMessageBox::information(this, i18n("Please select a button to delete from the list"));
    return;
  }

  int row = ui.tvTabContent->currentIndex().row();
  if (!setContainer->moveButtonUp(ui.cbSets->currentText(), ui.cbTabs->currentText(), button)) {
    KMessageBox::sorry(this, i18n("Button could not be moved up. Is it already at the top?"));
  }
  else {
    ui.tvTabContent->selectRow(row-1);
    emit changed(true);
  }
}


void KeyboardConfiguration::buttonDown()
{
  KeyboardButton *button = static_cast<KeyboardButton*>(ui.tvTabContent->currentIndex().internalPointer());
  if (!button) {
    KMessageBox::information(this, i18n("Please select a button to delete from the list"));
    return;
  }

  int row = ui.tvTabContent->currentIndex().row();
  if (!setContainer->moveButtonDown(ui.cbSets->currentText(), ui.cbTabs->currentText(), button)) {
    KMessageBox::sorry(this, i18n("Button could not be moved down. Is it already at the bottom?"));
  }
  else {
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
    ui.tvTabContent->setModel(0);
    return;
  }

  QString currentSet = ui.cbSets->currentText();
  QString currentTab = ui.cbTabs->currentText();
  ui.tvTabContent->setModel(setContainer->getTab(currentSet, currentTab));
}


QDomElement KeyboardConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  //general
  QDomElement caseSensitivityElem = doc->createElement("caseSensitivity");
  caseSensitivityElem.appendChild(doc->createTextNode(ui.cbCaseSensitivity->isChecked() ? "1" : "0"));
  configElem.appendChild(caseSensitivityElem);

  QDomElement selectedSetElem = doc->createElement("selectedSet");
  selectedSetElem.appendChild(doc->createTextNode(ui.cbSets->currentText()));
  configElem.appendChild(selectedSetElem);

  QDomElement geometryElement = doc->createElement("keyboardGeometry");
  QDomElement geometryPositionElement = doc->createElement("position");
  QDomElement geometryPositionXElement = doc->createElement("x");
  geometryPositionXElement.appendChild(doc->createTextNode(QString::number(m_keyboardPosition.x())));
  QDomElement geometryPositionYElement = doc->createElement("y");
  geometryPositionYElement.appendChild(doc->createTextNode(QString::number(m_keyboardPosition.y())));
  geometryPositionElement.appendChild(geometryPositionXElement);
  geometryPositionElement.appendChild(geometryPositionYElement);
  geometryElement.appendChild(geometryPositionElement);

  QDomElement geometrySizeElement = doc->createElement("size");
  QDomElement geometrySizeWElement = doc->createElement("width");
  geometrySizeWElement.appendChild(doc->createTextNode(QString::number(m_keyboardSize.width())));
  QDomElement geometrySizeHElement = doc->createElement("height");
  geometrySizeHElement.appendChild(doc->createTextNode(QString::number(m_keyboardSize.height())));
  geometrySizeElement.appendChild(geometrySizeWElement);
  geometrySizeElement.appendChild(geometrySizeHElement);
  geometryElement.appendChild(geometrySizeElement);

  configElem.appendChild(geometryElement);

  //numpad
  QDomElement numpadElem= doc->createElement("numpad");

  QDomElement showNumPadElem = doc->createElement("showNumpad");
  showNumPadElem.appendChild(doc->createTextNode(ui.cbShowNumpad->isChecked() ? "1" : "0"));
  numpadElem.appendChild(showNumPadElem);

  configElem.appendChild(numpadElem);

  //sets
  QDomElement setsElem = setContainer->serialize(doc);
  configElem.appendChild(setsElem);

  if (!ui.cbSets->currentText().isEmpty())
    storedSet = setContainer->findSet(ui.cbSets->currentText());

  if (QThread::currentThread() == qApp->thread()) {
    kDebug() << "Calling rebuild gui";
    commandManager->rebuildGui();
  }

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

  QDomElement geometryElement = elem.firstChildElement("keyboardGeometry");
  QDomElement geometryPositionElement = geometryElement.firstChildElement("position");
  QDomElement geometryPositionXElement = geometryPositionElement.firstChildElement("x");
  QDomElement geometryPositionYElement = geometryPositionElement.firstChildElement("y");
  QDomElement geometrySizeElement = geometryElement.firstChildElement("size");
  QDomElement geometrySizeWElement = geometrySizeElement.firstChildElement("width");
  QDomElement geometrySizeHElement = geometrySizeElement.firstChildElement("height");

  if (!geometryPositionXElement.isNull() && !geometryPositionYElement.isNull())
    m_keyboardPosition = QPoint(geometryPositionXElement.text().toInt(), geometryPositionYElement.text().toInt());
  if (!geometrySizeWElement.isNull() && !geometrySizeHElement.isNull())
    m_keyboardSize = QSize(geometrySizeWElement.text().toInt(), geometrySizeHElement.text().toInt());

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

  if (QThread::currentThread() == qApp->thread()) {
    kDebug() << "Calling rebuild gui";
    commandManager->rebuildGui();
  }

  return true;
}


void KeyboardConfiguration::defaults()
{
  ui.cbCaseSensitivity->setChecked(false);
  ui.cbShowNumpad->setChecked(true);

  QString selectedSet = "Basic";

  int index = ui.cbSets->findText(selectedSet);
  if (index != -1) {
    ui.cbSets->setCurrentIndex(index);
    refreshCbTabs();
  }
}


KeyboardConfiguration::~KeyboardConfiguration()
{
  //do not delete my precious!
  ui.tvTabContent->setModel(0);
}
