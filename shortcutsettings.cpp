//
// C++ Implementation: shortcutsettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shortcutsettings.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QTableWidgetSelectionRange>
#include "shortcut.h"
#include "shortcutcontrol.h"
#include "selectshortcutbutton.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param shortcutControl The control class which manages the shortcuts
 * @param parent The parent of the Systemwidget
 */
ShortcutSettings::ShortcutSettings(ShortcutControl *shortcutControl, QWidget* parent): 
	SystemWidget(tr("Tastenkürzel"), QIcon(":/images/icons/preferences-desktop-keyboard-shortcuts.svg"), 
	tr("Hier können Sie Shortcuts konfigurieren die von simon als Reaktion auf ein Schlüsselwort ausgelöst werden"), parent)
{
	Shortcut::initKeys();
	this->shortcutControl = shortcutControl;
	this->shortcutList = new ShortcutList();
	ui.setupUi(this);

	this->oldRow = ui.twShortcuts->currentRow();
	justChanged = false;

	connect(ui.twShortcuts, SIGNAL(currentCellChanged(int,int,int,int)), this, 
		SLOT(cellChanged(int,int,int, int)));
	connect(ui.twShortcuts, SIGNAL(enterPressed()), this, SLOT(applyName()));
	connect(ui.pbAddShortcut, SIGNAL(clicked()), this, SLOT(addShortcut()));
	connect(ui.pbDeleteShortcut, SIGNAL(clicked()), this, SLOT(deleteShortcut()));
}


/**
 * \brief Adds a new line to the TableWidget and a new (empty) shortcut to the local list
 * \author Peter Grasch
 */
void ShortcutSettings::addShortcut()
{
	if (!applyName()) return;
	justChanged = true;
	int currentRow = ui.twShortcuts->currentRow();
	if (currentRow == -1) currentRow = 0;
	
	if (ui.twShortcuts->rowCount() == 0) ui.twShortcuts->setRowCount(1);
	else ui.twShortcuts->insertRow(currentRow);
	ui.twShortcuts->setItem(currentRow, 0, new QTableWidgetItem());
	
	ui.twShortcuts->setItem(currentRow, 1, new QTableWidgetItem());
	ui.twShortcuts->setCurrentCell(currentRow,0);
	ui.twShortcuts->editItem(ui.twShortcuts->item(currentRow, 0));
	this->shortcutList->insert(currentRow, new Shortcut("", 0, 0, 0, 0, '_'));
}

/**
 * \brief Deletes the currently selected Shortcut
 * \author Peter Grasch
 */
void ShortcutSettings::deleteShortcut()
{
	justChanged=true;
	int currentRow = ui.twShortcuts->currentRow();
	if (currentRow == -1)
	{
		QMessageBox::information(this, tr("Nichts ausgewählt"), tr("Bitte selektieren Sie eine Tastenkombination aus der Liste um sie zu löschen."));
		return;
	}
	
	ui.twShortcuts->removeRow(currentRow);
	shortcutList->removeAt(currentRow);
}

/**
 * \brief Reselects the first cell on the row given by rowToSelect (member)
 * \author Peter Grasch
 */
void ShortcutSettings::reselect()
{
	justChanged=true;
	ui.twShortcuts->setCurrentCell(rowToSelect, 0);
	justChanged=false;
}

/**
 * \brief Reacts on changing the cell by hiding/displaying the SelectShortcutButtons
 * \author peter Grasch
 * 
 * If we go to column 1 (the column with the shortcuts) we toggle the button right-away;
 * thus giving the user a one-click-start method on adding/changing a shortcut
 * 
 * @param newrow The row to change to
 * @param newcolumn The column to change to
 * @param oldrow The previous row
 */
void ShortcutSettings::cellChanged(int newrow, int newcolumn, int oldrow, int oldcolumn)
{
	this->oldRow = oldrow;
	if (ui.twShortcuts->cellWidget(oldrow, 1))
	{
		QString text = "";
		
		if (dynamic_cast<SelectShortcutButton*>(ui.twShortcuts->cellWidget(oldrow, 1)))
			text = dynamic_cast<SelectShortcutButton*>(
						ui.twShortcuts->cellWidget(oldrow, 1))->text();
		
		ui.twShortcuts->setItem(oldrow, 1, new QTableWidgetItem(text));
		ui.twShortcuts->removeCellWidget(oldrow, 1);
	}
	

	
	if (!ui.twShortcuts->item(newrow, 1)) return;
	
	QString shortcutText = ui.twShortcuts->item(newrow, 1)->text();


	SelectShortcutButton *ssb = new SelectShortcutButton(ui.twShortcuts->item(newrow, 0)->text(), 
					shortcutText);

	connect(ssb, SIGNAL(shortcutCreated(Shortcut*)), this, SLOT(insertShortcut(Shortcut*)));
	if (newcolumn == 1) ssb->setChecked(true);
	
	ui.twShortcuts->setCellWidget(newrow, 1, ssb);

	applyName(oldrow, oldcolumn);
}


/**
 * \brief Checks if everything is okay (name not already used / not empty)
 * \author Peter Grasch
 * @param row The row to check
 * @param column The column to check
 * @return True if it is ok to continiue with what ever we were doing (i.e. nothing wrong)
 */
bool ShortcutSettings::applyName(int row, int column)
{
	if (justChanged) { justChanged = false; return true; }
	QCoreApplication::processEvents();

	if (row == -1) row = ui.twShortcuts->currentRow();
	if (column == -1) column = ui.twShortcuts->currentColumn();

	if (column != 0) return true;
	if ((row == -1) || (!ui.twShortcuts->item(row, 0))) return true;
	

	
	QString newName = ui.twShortcuts->item(row, 0)->text();
	
	bool alreadyExists=nameExists(newName, row);
	
	bool remove = false;
	bool rename = false;
	if (alreadyExists)
		if (QMessageBox::question(this, tr("Bereits vorhanden"), tr("Ein Kürzel mit dem Namen \"%1\" ist bereits vergeben. Sie können nicht mehrere Kürzel mit dem selben Namen einfügen.\n\nWollen Sie dieses Kürzel löschen?").arg(newName), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) remove = true;
		else rename = true;

	if (newName.isEmpty()) {
		QMessageBox::information(this, tr("Leerer Name"), tr("Das Kürzel hat keinen Namen.\n\nDer Name ist der \"Auslöser\" der Kombination. Bitte definieren Sie einen Namen."));

		rename = true;
	}
	
	bool reset = true;
	if (remove)
	{
		justChanged=true;
		ui.twShortcuts->removeRow(row);
		shortcutList->removeAt(row);
	} else if (rename)
	{
		ui.twShortcuts->setItem(row, 0, new QTableWidgetItem(""));

		ui.twShortcuts->editItem(ui.twShortcuts->item(row, 0));
	} else {
		reset = false;
		Shortcut *temp = shortcutList->at(row);
		if (!temp) {
			QMessageBox::critical(this, "Internal Error", "Please contact your WTF-Manager.");
			return true;
		}
		temp->setName(newName);
		shortcutList->replace(row, temp);
		return true;
	}
	if (reset) {
		this->rowToSelect = row; //really ugly workaround for the "select-before-change" problem
		//normally it would go like this
		//The item a hasn't been correctly entered and the user tries to select item B;
		//We select item A again (if we want to re-edit it), the process continious and selects
		//item B (which was the original intention of the user);
		//
		//this workaround does nothing more than to (re-) select the item again after a few ms
		QTimer::singleShot(5, this, SLOT(reselect()));
	}
	return false;
}

/**
 * \brief Returns if the name given already exists
 * \author Peter Grasch
 * 
 * We can specify a rowToExclude - this will most likely be used to exclude the row the item-to-check lies
 * in.
 * If we want to check the following example:
 * 	+-----+--------------------------+
 * 	|  0  |  This                    |
 * 	|  1  |  is_ (currently editing) |
 * 	|  2  |  a                       |
 * 	|  3  |  test                    |
 * 	+-----+--------------------------+
 * and we are checking if there is already an "is" in the list we will omit row 1 as otherwise the function
 * would find the "is" in line 1 and return true;
 * 
 * @param newName The name to search
 * @param rowToExclude We can exclude one row from the search
 * @return 
 */
bool ShortcutSettings::nameExists(QString newName, int rowToExclude)
{
	for (int i=0; i < shortcutList->count(); i++)
	{
		if ((shortcutList->at(i)->getName() == newName) && (i != rowToExclude))
		{
			return true;
		}
	}
	return false;
}

/**
 * \brief Inserts the given shortcut into the list
 * 
 * The function is smart enough to determine if we want to replace an existing command or want to insert
 * a new command and does so accordingly.
 * 
 * \author Peter Grasch
 * @param shortcut The shortcut to insert (it won't be checked if the pointer is valid!)
 */
void ShortcutSettings::insertShortcut(Shortcut *shortcut)
{
	int currentRow = ui.twShortcuts->currentRow();
	if (currentRow == -1) return;  //odd error, but you never know...
	
	if (shortcutList->count() != ui.twShortcuts->rowCount()) //we insert a NEW shortcut
	{
		shortcutList->insert(currentRow, shortcut);
	} else
	{	//replace the command at the current row
		shortcutList->replace(currentRow, shortcut);
	}
}


/**
 * \brief Applys the list of commands
 * \author Peter Grasch
 * @return Success
 */
bool ShortcutSettings::apply()
{
	shortcutControl->clearShortcutList();
	shortcutControl->setShortcuts(new ShortcutList(*shortcutList));
	shortcutControl->save();
	
	return true;
}


/**
 * \brief Copys the list of given shortcuts and returns the copy
 * @param shortcuts The list to add
 * @return the new list
 */
ShortcutList* ShortcutSettings::copyList(ShortcutList *shortcuts)
{
	ShortcutList *newList = new ShortcutList();
	for (int i=0; i < shortcuts->count(); i++)
	{
		newList->append(new Shortcut(shortcuts->at(i)->getName(),
					     shortcuts->at(i)->getModifiers(),
					     shortcuts->at(i)->getActionKeys(),
					     shortcuts->at(i)->getFunctionKeys(),
					     shortcuts->at(i)->getMovementKeys(),
					     shortcuts->at(i)->getCharKey()));
	}
	return newList;
}


/**
 * \brief Loads the shortcuts and displays them
 * \author Peter Grasch
 * @return Success
 */
bool ShortcutSettings::init()
{
	ShortcutList *shortcuts = shortcutControl->getShortcuts();
	
	if (!shortcuts) return false;

	this->shortcutList=copyList(shortcuts);
	ui.twShortcuts->setRowCount(shortcutList->count());

	for (int i=0; i < shortcutList->count(); i++)
	{
		ui.twShortcuts->setItem(i, 0, new QTableWidgetItem(shortcutList->at(i)->getName()));
		ui.twShortcuts->setItem(i, 1, new QTableWidgetItem(shortcutList->at(i)->getDescription()));

	}

	ui.twShortcuts->resizeColumnToContents(0);
	ui.twShortcuts->resizeColumnToContents(1);

	return true;
}

/**
 * \brief Resets the Display (reverts the list to the last saved one)
 * \author Peter Grasch
 * @return Success
 */
bool ShortcutSettings::reset()
{
	ui.twShortcuts->setCurrentCell(-1,0);
	return init();
}
