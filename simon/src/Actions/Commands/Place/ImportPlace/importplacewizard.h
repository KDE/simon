//
// C++ Interface: importplacewizard
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTPLACEWIZARD_H
#define IMPORTPLACEWIZARD_H

#include <QWizard>


class IntroPlacePage;
class SelectPlacePage;
class Command;

/**
 * \class ImportPlaceWizard
 * \author Susanne Tschernegg
 * \date 03.09.2007
 * \version 0.1
 * \brief With this wizard the user can add a local or a remote place to the commands.
 */
class ImportPlaceWizard : public QWizard
{

    Q_OBJECT
		
	private:
        SelectPlacePage* selectPlacePage;

	QWizardPage* createIntroPlacePage();
	SelectPlacePage* createSelectPlacePage();

        QWizardPage* createFinishedPage();

public:
        ImportPlaceWizard(QWidget* parent=0);

        ~ImportPlaceWizard();

private slots:
	void createCommand(int);
signals:
   void commandCreated(const Command* command);

};

#endif
