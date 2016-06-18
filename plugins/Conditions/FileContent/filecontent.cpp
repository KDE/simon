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

#include "filecontent.h"
#include "createfilecontentwidget.h"
#include <QWidget>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QDebug>
#include <KPluginFactory>

K_PLUGIN_FACTORY( FileContentPluginFactory,
registerPlugin< FileContent >();
)

// K_EXPORT_PLUGIN( FileContentPluginFactory("simonfilecontent") )

FileContent::FileContent(QObject *parent, const QVariantList &args) :
    Condition(parent, args),
    watcher(new QFileSystemWatcher(this))
{
  m_pluginName = "simonfilecontentplugin.desktop";
  connect(watcher,SIGNAL(fileChanged(QString)), this, SLOT(checkFile()));
}

CreateConditionWidget* FileContent::getCreateConditionWidget(QWidget* parent)
{
    return new CreateFileContentWidget(parent);
}

QDomElement FileContent::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    QDomElement filenameElem = doc->createElement("filename");
    filenameElem.appendChild(doc->createTextNode(m_filename));

    QDomElement fileContentElem = doc->createElement("filecontent");
    fileContentElem.appendChild(doc->createTextNode(m_fileContent));

    QDomElement regExpElem = doc->createElement("filecontentregexp");
    regExpElem.appendChild(doc->createTextNode(m_fileContentIsRegularExpression ? "1" : "0"));

    elem.appendChild(filenameElem);
    elem.appendChild(fileContentElem);
    elem.appendChild(regExpElem);

    return elem;
}

QString FileContent::name()
{
    if (isInverted())
        return i18nc("%1 is filename, %2 is the content to look for", "'%1' does not have the content '%2'", m_filename, m_fileContent);
    else
        return i18nc("%1 is filename, %2 is the content to look for", "'%1' has the content '%2'", m_filename, m_fileContent);
}

bool FileContent::privateDeSerialize(QDomElement elem)
{
    QDomElement nameElement;

    //get the file name
    nameElement = elem.firstChildElement("filename");
    if (nameElement.isNull()) {
        qDebug() << "No file name specified!  Deserialization failure!";
        return false;
    }
    m_filename = nameElement.text();

    //get the file content
    nameElement = elem.firstChildElement("filecontent");
    if (nameElement.isNull()) {
        qDebug() << "No window name specified!  Deserialization failure!";
        return false;
    }
    m_fileContent = nameElement.text();

    //get whether or not the window name is a regular expression
    nameElement = elem.firstChildElement("filecontentregexp");
    if (nameElement.isNull()) {
        return false;
    }
    m_fileContentIsRegularExpression = (nameElement.text() == "1");

    if (m_fileContentIsRegularExpression)
        m_fileContentRegExp = QRegExp(m_fileContent);
    else
        m_fileContentRegExp = QRegExp(m_fileContent, Qt::CaseSensitive, QRegExp::FixedString);

    watcher->addPath(m_filename);
    qDebug() << "Adding watcher to: " << m_filename;
    
    checkFile();
    return true;
}

void FileContent::checkFile()
{
   //wait for write lock to be released
  QTimer::singleShot(100, this, SLOT(checkFileInternal()));
}

void FileContent::checkFileInternal()
{
    qDebug() << "Checking file: " << m_filename;
    QFile f(m_filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open file: " << m_filename;
        return;
    }

    bool newSatisfied = false;
    if (m_fileContentIsRegularExpression) {
        QString content = QString::fromUtf8(f.readAll());
        newSatisfied = (m_fileContentRegExp.indexIn(content) >= 0);
    } else {
        QByteArray start = f.read(m_fileContent.length());
	newSatisfied = (QString::fromUtf8(start) == m_fileContent);
    }
    bool changed = (newSatisfied != m_satisfied);
    m_satisfied = newSatisfied;
    qDebug() << "Satisfied: " << m_satisfied;
    if (changed)
        emit conditionChanged();
    
    //funnily enough, without the below code, the signal only fires
    //the first couple of times
    watcher->removePath(m_filename);
    watcher->addPath(m_filename);
}
#include "filecontent.moc"
