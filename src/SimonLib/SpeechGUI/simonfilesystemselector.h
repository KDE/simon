//
// C++ Interface: word
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMONFILESYSTEMSELECTOR_H
#define SIMONFILESYSTEMSELECTOR_H
#include <QString>
#include <QWidget>
#include <QFileDialog>
class SimonLineEdit;
class QToolButton;
/**
 *	@class SimonFileSystemSelector
 *	@brief LineEdit coupled with a selector button to select files / directories
 *
 *	@version 0.1
 *	@date 08.05.2007
 *	@author Peter Grasch
 */
class SimonFileSystemSelector : public QWidget {

Q_OBJECT

Q_PROPERTY(QString currentUrl READ text)

signals:
	void editingFinished();
	void urlChanged(QString);

protected:
	QString defaultSrc, dlgCaption, dir, filter;
	QFileDialog::Options options;
	SimonLineEdit *edit;
	QToolButton *button;
	QFileDialog::FileMode type;

private slots:
	void showOpenDialog();

public slots:
	QString text();
	void setText(QString text);

public:
	SimonFileSystemSelector(QString defaultSrc, QString dlgCaption, 
						QString dir,  QFileDialog::Options options,
						QString filter, QFileDialog::FileMode type,
						QWidget *parent=0);
	~SimonFileSystemSelector() {}
};

#endif
