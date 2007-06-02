//
// C++ Interface: importtrainingtexts
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGTEXTS_H
#define IMPORTTRAININGTEXTS_H

#include <QWidget>
#include <QWizard>
#include <QWizardPage>
#include <QMessageBox>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QPixmap>
#include <QRadioButton>
#include <QObjectList>

/**
 * \brief Guides the user through the process of adding a new trainingtext
 * 
 * This class shows a Wizard that walks the user through that process
 * 
 * \author Peter Grasch
 * \date 20.05.2007
 * \version 0.1
*/
class ImportTrainingTexts : public QWizard{
	
	Q_OBJECT
	enum { PageIntro, PageSource, PageFile, PageInternet, PageWorking, PageFinished };
private:
	QFileDialog *fd;
public:
    ImportTrainingTexts(QWidget *parent=0);
    
    void start();
    
    
    //creating the pages for the wizard

    QWizardPage* createIntroPage();
    QWizardPage* createSourcePage();
    QWizardPage* createRemoteImportPage();
    QWizardPage* createLocalImportPage();
    QWizardPage* createWorkingPage();
    QWizardPage* createFinishedPage();

    ~ImportTrainingTexts();

public slots:
	void setLocalSourceFile();


};

class SelectSourceWizardPage :  public QWizardPage {
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

class ImportLocalWizardPage : public QWizardPage {
	
public:
	ImportLocalWizardPage(QWidget* parent);
	void registerField(const QString & name, QWidget * widget, 
			   const char * property = 0, const char * changedSignal = 0 );
	int nextId() const { return 4; }
};

class ImportRemoteWizardPage : public QWizardPage {
		
	public:
		ImportRemoteWizardPage(QWidget* parent);
		void registerField(const QString & name, QWidget * widget, 
				   const char * property = 0, const char * changedSignal = 0 );
};

#endif
