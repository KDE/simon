//
// C++ Implementation: configurationdialog
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QObject>

class ConfigurationDialog : public QObject
{
	Q_OBJECT

// 	private:
// 		bool addSystemWidget(SystemWidget *widget);

	public:
		ConfigurationDialog(QWidget *parent);

		void show();

		~ConfigurationDialog();

};

#endif
