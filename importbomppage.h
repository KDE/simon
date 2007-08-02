//
// C++ Interface: importbomppage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTBOMPPAGE_H
#define IMPORTBOMPPAGE_H

#include <QWizardPage>
#include <QVariant>

class QString;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportBOMPPage : public QWizardPage
{
Q_OBJECT

private slots:
    void setFile();
	
public:
    ImportBOMPPage(QWidget* parent);
	QString getFileName()
	 { return field("fileName").toString(); }

    ~ImportBOMPPage();

};

#endif
