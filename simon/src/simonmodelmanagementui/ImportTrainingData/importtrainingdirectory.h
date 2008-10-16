/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef IMPORTTRAININGDIRECTORY_H
#define IMPORTTRAININGDIRECTORY_H

#include <QWizard>

class QWidget;
class QWizardPage;
class ImportTrainingDirectoryIntroPage;
class ImportTrainingDirectoryWorkingPage;


/**
  \class ImportTrainingDirectory
  \author Peter Grasch
  \brief The wizard to import a training-directory

  The wizard is there to migrated tagged-audio-data into the language
  model.
  It does that using the filename to determine what the user said;
  e.g.: this_is_a_test.wav
  would be the line: *\/this_is_a_test.wav THIS IS A TEST
  in the prompts file and so on.

  The wizard walks the directory recursive;


  Extends QWizard;
  
*/
class ImportTrainingDirectory : public QWizard {
	Q_OBJECT

private:
	int prevId;
	ImportTrainingDirectoryIntroPage* createIntroPage();
	ImportTrainingDirectoryWorkingPage* createWorkingPage();
	QWizardPage* createFinishedPage();
public:
    ImportTrainingDirectory(QWidget *parent=0);

    ~ImportTrainingDirectory()
	{
	}

};

#endif
