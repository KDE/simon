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
#include <QGridLayout>
#include <QLineEdit>
#include <QProgressBar>
#include <QDir>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QPushButton>
#include <QPixmap>
#include <QRadioButton>
#include "quickdownloader.h"
#include "xmltrainingtextlist.h"
#include "xmltrainingtext.h"
#include <QObjectList>
#include <QFileInfo>

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
	int prevId;
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
	void idChanged(int id);

};

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

class ImportWorkingWizardPage : public QWizardPage {
	Q_OBJECT 
	public slots:
		void startImport(QString path);
		void processText(QString path);
		void parseFile(QString path);
	public:
		ImportWorkingWizardPage(QWidget* parent);
};

#endif
