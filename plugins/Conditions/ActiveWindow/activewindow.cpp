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

#include "activewindow.h"
#include <KDE/KDebug>
#include "createactivewindowwidget.h"
#include <QWidget>

K_PLUGIN_FACTORY( ActiveWindowPluginFactory,
registerPlugin< ActiveWindow >();
)

K_EXPORT_PLUGIN( ActiveWindowPluginFactory("simonactivewindow") )

ActiveWindow::ActiveWindow(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
    m_processName = "";
    m_windowName = "";
    m_windowNameIsRegularExpression = false;
    m_windowNameRegExp = QRegExp();
    m_currentProcessName = "";
    m_currentWindowName = "";

    m_pluginName = "simonactivewindowplugin.desktop";
}

CreateConditionWidget* ActiveWindow::getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget* parent)
{
    return new CreateActiveWindowWidget(compoundCondition, parent);
}

QDomElement ActiveWindow::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    QDomElement processNameElem = doc->createElement("processname");
    processNameElem.appendChild(doc->createTextNode(m_processName));

    QDomElement windowNameElem = doc->createElement("windowname");
    windowNameElem.appendChild(doc->createTextNode(m_windowName));

    QDomElement regExpElem = doc->createElement("windownameregexp");
    regExpElem.appendChild(doc->createTextNode(m_windowNameIsRegularExpression ? "1" : "0"));

    elem.appendChild(processNameElem);
    elem.appendChild(windowNameElem);
    elem.appendChild(regExpElem);

    return elem;
}

QString ActiveWindow::name()
{
    if (isInverted())
        return i18nc("%1 is process name, %2 is window name", "'%1' does not have the active window with title '%2'", m_processName, m_windowName);
    else
        return i18nc("%1 is process name, %2 is window name", "'%1' has the active window with title '%2'", m_processName, m_windowName);
}

bool ActiveWindow::privateDeSerialize(QDomElement elem)
{
    QDomElement nameElement;

    //get the process name
    nameElement = elem.firstChildElement("processname");
    if (nameElement.isNull())
    {
        kDebug() << "No processes name specified!  Deserialization failure!";
        return false;
    }
    m_processName = nameElement.text();

    //get the window name
    nameElement = elem.firstChildElement("windowname");
    if (nameElement.isNull())
    {
        kDebug() << "No window name specified!  Deserialization failure!";
        return false;
    }
    m_windowName = nameElement.text();

    //get whether or not the window name is a regular expression
    nameElement = elem.firstChildElement("windownameregexp");
    if (nameElement.isNull())
    {
        kDebug() << "No window name regexp specified!  Deserialization failure!";
        return false;
    }
    m_windowNameIsRegularExpression = (nameElement.text() == "1");

    if (m_windowNameIsRegularExpression)
    {
        m_windowNameRegExp = QRegExp(m_windowName);
        kDebug() << "RegExp '" << m_windowNameRegExp.pattern() << (m_windowNameRegExp.isValid() ? "' is Valid!" : "' is not Valid!");
    }
    else
    {
        m_windowNameRegExp = QRegExp(m_windowName, Qt::CaseSensitive, QRegExp::FixedString);
    }

    //connect to the ProcessInfo instance
    ProcessInfo* processInfo = ProcessInfo::instance();

    connect(processInfo, SIGNAL(activeWindowProcessChanged(QString)),
            this, SLOT(checkActiveWindowProcess(QString)));
    connect(processInfo, SIGNAL(activeWindowTitleChanged(QString)),
            this, SLOT(checkActiveWindowTitle(QString)));

    return true;
}

void ActiveWindow::checkActiveWindowProcess(QString processName)
{
    if (processName != m_currentProcessName)
    {
        m_currentProcessName = processName;
        checkBothNames();
    }
}

void ActiveWindow::checkActiveWindowTitle(QString title)
{
    if (title != m_currentWindowName)
    {
        m_currentWindowName = title;
        checkBothNames();
    }
}

void ActiveWindow::checkBothNames()
{
    if (m_satisfied)
    {
        if ((m_currentProcessName != m_processName)
                || (m_windowNameRegExp.indexIn(m_currentWindowName) < 0))
        {
            m_satisfied = false;
            kDebug() << name() << " is unsatisfied!";
            emit conditionChanged();
        }
    }
    else
    {
        if ((m_currentProcessName == m_processName)
                && (m_windowNameRegExp.indexIn(m_currentWindowName) >= 0))
        {
            m_satisfied = true;
            kDebug() << name() << " is satisfied!";
            emit conditionChanged();
        }
    }
}
