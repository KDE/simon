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
 * \class ImportDictWiktionaryPage
 * \brief Lets the user select a local or remote wiktionary to import
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
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
