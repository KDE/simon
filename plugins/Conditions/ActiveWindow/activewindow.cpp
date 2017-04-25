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
#include "createactivewindowwidget.h"
#include <QWidget>
#include <QDebug>
#include <KPluginFactory>

K_PLUGIN_FACTORY( ActiveWindowPluginFactory,
registerPlugin< ActiveWindow >();
)

// K_EXPORT_PLUGIN( ActiveWindowPluginFactory("simonactivewindow") )

ActiveWindow::ActiveWindow(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
    m_windowName = "";
    m_windowNameIsRegularExpression = false;
    m_windowNameRegExp = QRegExp();
    m_currentWindowName = "";

    m_pluginName = "simonactivewindowplugin.desktop";
}

CreateConditionWidget* ActiveWindow::getCreateConditionWidget(QWidget* parent)
{
    return new CreateActiveWindowWidget(parent);
}

QDomElement ActiveWindow::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    QDomElement windowNameElem = doc->createElement("windowname");
    windowNameElem.appendChild(doc->createTextNode(m_windowName));

    QDomElement regExpElem = doc->createElement("windownameregexp");
    regExpElem.appendChild(doc->createTextNode(m_windowNameIsRegularExpression ? "1" : "0"));

    elem.appendChild(windowNameElem);
    elem.appendChild(regExpElem);

    return elem;
}

QString ActiveWindow::name()
{
    if (isInverted())
        return i18nc("%1 is window name", "The active window title is not '%1'", m_windowName);
    else
        return i18nc("%1 is window name", "The active window title is '%1'", m_windowName);
}

bool ActiveWindow::privateDeSerialize(QDomElement elem)
{
    QDomElement nameElement;

    //get the window name
    nameElement = elem.firstChildElement("windowname");
    if (nameElement.isNull())
    {
        qDebug() << "No window name specified!  Deserialization failure!";
        return false;
    }
    m_windowName = nameElement.text();

    //get whether or not the window name is a regular expression
    nameElement = elem.firstChildElement("windownameregexp");
    if (nameElement.isNull())
    {
        qDebug() << "No window name regexp specified!  Deserialization failure!";
        return false;
    }
    m_windowNameIsRegularExpression = (nameElement.text() == "1");

    if (m_windowNameIsRegularExpression)
    {
        m_windowNameRegExp = QRegExp(m_windowName, Qt::CaseInsensitive);
        qDebug() << "RegExp '" << m_windowNameRegExp.pattern() << (m_windowNameRegExp.isValid() ? "' is Valid!" : "' is not Valid!");
    }
    else
    {
        m_windowNameRegExp = QRegExp(m_windowName, Qt::CaseSensitive, QRegExp::FixedString);
    }

    connect(ProcessInfo::instance(), SIGNAL(activeWindowTitleChanged(QString)),
            this, SLOT(checkActiveWindowTitle(QString)));

    return true;
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
        if (m_windowNameRegExp.indexIn(m_currentWindowName) < 0)
        {
            m_satisfied = false;
            qDebug() << name() << " is unsatisfied!";
            emit conditionChanged();
        }
    }
    else
    {
        if (m_windowNameRegExp.indexIn(m_currentWindowName) >= 0)
        {
            m_satisfied = true;
            qDebug() << name() << " is satisfied!";
            emit conditionChanged();
        }
    }
}
#include "activewindow.moc"
