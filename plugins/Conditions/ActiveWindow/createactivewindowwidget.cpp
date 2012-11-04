/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "createactivewindowwidget.h"
#include "activewindow.h"
#include <simoncontextdetection/contextmanager.h>
#include <simonuicomponents/selectprogramdialog.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QStringList>

CreateActiveWindowWidget::CreateActiveWindowWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("Active Window Condition"));
  setWindowIcon(KIcon("window-duplicate"));

  connect(ui.leWindowTitle, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.cbRegExp, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
}

bool CreateActiveWindowWidget::isComplete()
{
    bool regExpOK = true;

    if (ui.cbRegExp->isChecked())
    {
        ui.lbRegExpValid->setEnabled(true);
        regExpOK = QRegExp(ui.leWindowTitle->text()).isValid();
        ui.lbRegExpValid->setText(regExpOK ? i18n("Regular expression is valid") : i18n("Regular expression is invalid"));
    }
    else
    {
        ui.lbRegExpValid->setEnabled(false);
        ui.lbRegExpValid->setText("");
    }

    return !(ui.leWindowTitle->text().isEmpty())
            && regExpOK;
}

bool CreateActiveWindowWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  ActiveWindow *activeWindow = dynamic_cast<ActiveWindow*>(condition);
  if (!activeWindow) return false;

  ui.leWindowTitle->setText(activeWindow->getWindowName());
  ui.cbRegExp->setChecked(activeWindow->getWindowNameIsRegExp());
  return true;
}


Condition* CreateActiveWindowWidget::createCondition( QDomDocument* doc, QDomElement& conditionElem )
{
    conditionElem.setAttribute("name", "simonactivewindowplugin.desktop");

    QDomElement windowElem = doc->createElement("windowname");
    windowElem.appendChild(doc->createTextNode(ui.leWindowTitle->text()));
    conditionElem.appendChild(windowElem);

    QDomElement regExpElem = doc->createElement("windownameregexp");
    regExpElem.appendChild(doc->createTextNode(ui.cbRegExp->isChecked() ? "1" : "0"));
    conditionElem.appendChild(regExpElem);

    return ContextManager::instance()->getCondition(conditionElem);
}


CreateActiveWindowWidget::~CreateActiveWindowWidget()
{
}
