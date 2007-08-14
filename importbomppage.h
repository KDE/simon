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
 * \class ImportBOMPPage
 * \brief WizardPage to select a BOMP Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
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
	int nextId() const { return 4; }

    ~ImportBOMPPage();

};

#endif
