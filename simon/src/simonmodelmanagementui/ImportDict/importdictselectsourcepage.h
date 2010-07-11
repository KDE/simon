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

#ifndef SIMON_IMPORTDICTSELECTSOURCEPAGE_H_2E59D0149B434858A2BAF8D65A597942
#define SIMON_IMPORTDICTSELECTSOURCEPAGE_H_2E59D0149B434858A2BAF8D65A597942

#include <QWizardPage>
#include "ui_importdictselectsourcepage.h"

class QRadioButton;
/**
 * \class ImportDictSelectSourcePage
 * \brief Displays a WizardPage to let the user select the type of dict. to import
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
 */
class ImportDictSelectSourcePage : public QWizardPage
{
  Q_OBJECT

    protected:
    Ui::ImportDictSelectSourcePage ui;

  public:
    int nextId() const;
    ImportDictSelectSourcePage(QWidget* parent);

    ~ImportDictSelectSourcePage();

};
#endif
