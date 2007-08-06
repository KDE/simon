//
// C++ Interface: importdictwiktionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTWIKTIONARYPAGE_H
#define IMPORTDICTWIKTIONARYPAGE_H

#include <QWizardPage>

class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportDictWiktionaryPage : public QWizardPage
{
Q_OBJECT

private:
	QListWidget *remoteList;
	QLineEdit *leWikiPath;
	QPushButton *pbWikiSelectFolder;
	QLabel *lbWikiPath;
	
private slots:
    void resambleImportLocal(bool isTru);
    void loadList();
    void importList(QString list);
    void setFile();


public:
    ImportDictWiktionaryPage(QWidget* parent);
	bool isComplete() const;
    QString getPath();

    ~ImportDictWiktionaryPage();

};

#endif
