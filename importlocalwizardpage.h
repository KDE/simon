//
// C++ Interface: importlocalwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTLOCALWIZARDPAGE_H
#define IMPORTLOCALWIZARDPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QVariant>
#include <QFileDialog>
#include <QDir>

/**
 * \class ImportLocalWizardPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Asks the user for the file to import
 * 
 * Extends QWizardpage; Uses the QT-Mandatory-field mechanism
 */
class ImportLocalWizardPage : public QWizardPage {
	Q_OBJECT
	private:
		QLineEdit *lePath;
	public slots:
		void setLocalSourceFile();
	public:
		ImportLocalWizardPage(QWidget* parent);
		void setPathEdit(QLineEdit *lePath) { this->lePath = lePath; }
		void registerField(const QString & name, QWidget * widget, 
				   const char * property = 0, const char * changedSignal = 0 );
		int nextId() const {
			return 4;
		}
		QVariant getField(QString name) { return field(name);}
};


#endif
