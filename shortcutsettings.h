//
// C++ Interface: shortcutsettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHORTCUTSETTINGS_H
#define SHORTCUTSETTINGS_H

#include "systemwidget.h"
#include "ui_shortcutsettingsdlg.h"
#include "shortcut.h"


class ShortcutControl;

/**
 * \class ShortcutSettigns
 * \brief Systemwidget to manage the shortcuts simon sends
 *
 * \note This is NOT the class to configure simon-specific shortcuts (e.g. CTRL+W for wordlist) but rather the management console to define shortcuts like "Close window"=>ALT+F4
 *
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
*/
class ShortcutSettings : public SystemWidget
{
Q_OBJECT
private:
	Ui::ShortcutSettingsDlg ui;

	int rowToSelect;

	int oldRow;
	bool justChanged;
	ShortcutControl *shortcutControl;
	ShortcutList *shortcutList;

	ShortcutList* copyList(ShortcutList *shortcuts);
	bool nameExists(QString newName, int rowToExclude=-1);

public:
        ShortcutSettings(QWidget* parent);
	bool isComplete();
        ~ShortcutSettings() { }

private slots:
	void cellChanged(int newrow, int newcolumn, int oldrow, int oldcolumn);
	void reselect();
	
public slots:
	void insertShortcut(Shortcut *shortcut);
	bool applyName(int row=-1, int column=-1);

	void addShortcut();
	void deleteShortcut();
        bool apply();
        bool init();
        bool reset();

};

#endif
