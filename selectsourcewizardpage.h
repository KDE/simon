//
// C++ Interface: selectsourcewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTSOURCEWIZARDPAGE_H
#define SELECTSOURCEWIZARDPAGE_H

#include <QWizardPage>
#include <QRadioButton>

class SelectSourceWizardPage :  public QWizardPage {
	Q_OBJECT

	private:
		QRadioButton *local;
		QRadioButton *remote;
	
	public:
		SelectSourceWizardPage(QWidget* parent);
		int nextId() const;
		QRadioButton* getLocal() const { return local; }
		QRadioButton* getRemote() const { return remote; }
		
		void setLocal(QRadioButton *local) { this->local = local; }
		void setRemote(QRadioButton *remote) { this->remote = remote; }
};

#endif
