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

  connect(ui.leProgramName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leWindowTitle, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.pbLocateProgram, SIGNAL(clicked()), this, SLOT(processFileDialog()));
  connect(ui.cbRegExp, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
}

void CreateActiveWindowWidget::processFileDialog()
{
    SelectProgramDialog *dlg = new SelectProgramDialog(this);

    if (dlg->selectCommand())
    {
        ui.leProgramName->setText(dlg->getExecPath().split(QRegExp("/+|\\\\+")).back());
    }

    dlg->deleteLater();
}

bool CreateActiveWindowWidget::isComplete()
{
    bool regExpOK = true;

    if (ui.cbRegExp->isChecked())
    {
        ui.lbRegExpValid->setEnabled(true);
        regExpOK = QRegExp(ui.leWindowTitle->text()).isValid();
        ui.lbRegExpValid->setText(regExpOK ? i18n("Regular Expression is Valid") : i18n("Regular Expression is Invalid!"));
    }
    else
    {
        ui.lbRegExpValid->setEnabled(false);
        ui.lbRegExpValid->setText("");
    }

    return !(ui.leProgramName->text().isEmpty() || ui.leWindowTitle->text().isEmpty())
            && regExpOK;
}

bool CreateActiveWindowWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  ActiveWindow *activeWindow = dynamic_cast<ActiveWindow*>(condition);
  if (!activeWindow) return false;

  ui.leProgramName->setText(activeWindow->getProcessName());
  ui.leWindowTitle->setText(activeWindow->getWindowName());
  ui.cbRegExp->setChecked(activeWindow->getWindowNameIsRegExp());
  ui.cbInverted->setChecked(activeWindow->isInverted());
  return true;
}


Condition* CreateActiveWindowWidget::createCondition()
{
    kDebug() << "Creating Process Opened Condition";
    QDomDocument doc;
    QDomElement conditionElem = doc.createElement("condition");
    conditionElem.setAttribute("name", "simonactivewindowplugin.desktop");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode(ui.cbInverted->isChecked() ? "1" : "0"));
    conditionElem.appendChild(invertElem);

    QDomElement programElem = doc.createElement("processname");
    programElem.appendChild(doc.createTextNode(ui.leProgramName->text()));
    conditionElem.appendChild(programElem);

    QDomElement windowElem = doc.createElement("windowname");
    windowElem.appendChild(doc.createTextNode(ui.leWindowTitle->text()));
    conditionElem.appendChild(windowElem);

    QDomElement regExpElem = doc.createElement("windownameregexp");
    regExpElem.appendChild(doc.createTextNode(ui.cbRegExp->isChecked() ? "1" : "0"));
    conditionElem.appendChild(regExpElem);


    ContextManager* manager = ContextManager::instance();

    return manager->getCondition(conditionElem);
}


CreateActiveWindowWidget::~CreateActiveWindowWidget()
{
}
