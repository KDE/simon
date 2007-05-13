//
// C++ Interface: importdictview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTVIEW_H
#define IMPORTDICTVIEW_H


#include <QDialog>
#include <QFileDialog>
#include "ui_importdict.h"
#include "word.h"
#include "importdict.h"
/**
 * \class ImportDictView
 * \brief Provides a dialog for importing dictonaries
 * \author Peter Grasch
*/
class ImportDictView : public QDialog{
	Q_OBJECT
private:
	Ui::ImportView ui;
	ImportDict *import; //!< Underlying concept class
	WordList *list;  //!< Is filled later on with the retrieved List
public slots:
	void importDict();
	void openFileDialog();
	void finishedImporting();
	void openingFinished();
	void validatePath(QString path);
	void displayStatus(QString status);
	void displayProgress(int progress);
public:
    ImportDictView(QWidget *parent=0);
	WordList* getList() {return list;}
    ~ImportDictView();

};

#endif
