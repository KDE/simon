//
// C++ Interface: command
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTPLACEPAGE_H
#define SELECTPLACEPAGE_H
#include <QWizardPage>
#include <QUrl>

#include "ui_selectplacedlg.h"
#include "importplacewizard.h"

/**
 *	@class SelectPlacePage
 *	@brief Allows the user to select a local or remote place
 *
 *	@version 0.1
 *	@date 29.05.2008
 *	@author Peter Grasch
 */
class SelectPlacePage : public QWizardPage{

Q_OBJECT

private:
	Ui::SelectPlaceDlg ui;
private slots:
	void buildRemoteUrl();
	void parseRemoteUrl();
	
public:
	QString getName();
	SelectPlacePage(QWidget *parent=0);
	QUrl getUrl();
	void initializePage();
	bool isComplete() const;

};

#endif
