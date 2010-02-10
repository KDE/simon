/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_MULTIKCMVIEW_H_A545F3AE90134F99905B9FCCE971C8C0
#define SIMON_MULTIKCMVIEW_H_A545F3AE90134F99905B9FCCE971C8C0

#include <KCModule>
#include <QList>

#include "simonuicomponents_export.h"

class QString;
class KIcon;
class KTabWidget;

class SIMONUICOMPONENTS_EXPORT MultiKCMView : public KCModule
{
Q_OBJECT

private:
	KTabWidget *wgModules;
	QList<KCModule*> modules;
	QList<bool> moduleChangedState;
	void emitChanged();

private slots:
	void slotChanged(bool changed);

protected:
	void registerModule(KCModule*, const KIcon& icon, const QString& label);

public:
	MultiKCMView(QWidget* parent, const QVariantList& args=QVariantList());
	~MultiKCMView();

	void load();
	void save();
};

#endif

