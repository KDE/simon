//
// C++ Interface: importremotewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTREMOTEWIZARDPAGE_H
#define IMPORTREMOTEWIZARDPAGE_H

#include <QWizardPage>
#include <QString>
#include <QWidget>
#include <QListWidget>
#include "quickdownloader.h"
#include "xmltrainingtextlist.h"

class ImportRemoteWizardPage : public QWizardPage {
	Q_OBJECT
	private:
		QListWidget *list;
	public slots:
		void fetchList();
		void importList(QString path);
		
	public:
		ImportRemoteWizardPage(QWidget* parent);
		void setList(QListWidget* list) { this->list = list; }
		void registerField(const QString & name, QWidget * widget, 
				   const char * property = 0, const char * changedSignal = 0 );
		int nextId() const {
			return 4; 
		}
		QString getCurrentData() { 
			if (list && list->currentItem())
				return list->currentItem()->data(Qt::UserRole).toString(); 
			else return "";
		}
};

#endif
