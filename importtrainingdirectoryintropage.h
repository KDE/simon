//
// C++ Interface: importtrainingdirectoryintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGDIRECTORYINTROPAGE_H
#define IMPORTTRAININGDIRECTORYINTROPAGE_H

#include <QWizardPage>
#include <QVariant>

class QLineEdit;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportTrainingDirectoryIntroPage : public QWizardPage{
	Q_OBJECT
private:
	QLineEdit *leDirectory;
public slots:
	void setDir();
public:
	ImportTrainingDirectoryIntroPage(QWidget *parent=0);

	~ImportTrainingDirectoryIntroPage();
	void setPathEdit(QLineEdit *leDirectory) { this->leDirectory = leDirectory; }
	void registerField(const QString & name, QWidget * widget, 
				const char * property = 0, const char * changedSignal = 0 );
	QVariant getField(QString name) { return field(name);}

};

#endif
