/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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

#include "createfilecontentwidget.h"
#include "filecontent.h"

#include <simoncontextdetection/contextmanager.h>


CreateFileContentWidget::CreateFileContentWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("File Content Condition"));
  setWindowIcon(QIcon::fromTheme("document-save"));

  connect(ui.leFileContent, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.urFilename, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.cbRegExp, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
}

bool CreateFileContentWidget::isComplete()
{
    bool regExpOK = true;

    if (ui.cbRegExp->isChecked())
    {
        ui.lbRegExpValid->setEnabled(true);
        regExpOK = QRegExp(ui.leFileContent->text()).isValid();
        ui.lbRegExpValid->setText(regExpOK ? i18n("Regular expression is valid") : i18n("Regular expression is invalid"));
    } else {
        ui.lbRegExpValid->setEnabled(false);
        ui.lbRegExpValid->setText("");
    }

    return !(ui.leFileContent->text().isEmpty() || ui.urFilename->url().toLocalFile().isEmpty())
            && regExpOK;
}

bool CreateFileContentWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  FileContent *fileContent = dynamic_cast<FileContent*>(condition);
  if (!fileContent) return false;

  ui.leFileContent->setText(fileContent->getFileContent());
  ui.urFilename->setUrl(fileContent->getFilename());
  ui.cbRegExp->setChecked(fileContent->getFileContentIsRegExp());
  return true;
}


Condition* CreateFileContentWidget::createCondition( QDomDocument* doc, QDomElement& conditionElem )
{
    conditionElem.setAttribute("name", "simonfilecontentplugin.desktop");

    QDomElement filenameEleme = doc->createElement("filename");
    filenameEleme.appendChild(doc->createTextNode(ui.urFilename->url().toLocalFile()));
    conditionElem.appendChild(filenameEleme);

    QDomElement fileContentElem = doc->createElement("filecontent");
    fileContentElem.appendChild(doc->createTextNode(ui.leFileContent->text()));
    conditionElem.appendChild(fileContentElem);

    QDomElement regExpElem = doc->createElement("filecontentregexp");
    regExpElem.appendChild(doc->createTextNode(ui.cbRegExp->isChecked() ? "1" : "0"));
    conditionElem.appendChild(regExpElem);

    return ContextManager::instance()->getCondition(conditionElem);
}


CreateFileContentWidget::~CreateFileContentWidget()
{
}
