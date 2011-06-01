#include "processopenedcondition.h"
#include <KDE/KDebug>

K_PLUGIN_FACTORY( ProcessOpenedPluginFactory,
registerPlugin< ProcessOpenedCondition >();
)

K_EXPORT_PLUGIN( ProcessOpenedPluginFactory("simonprocessopenedcondition") )

ProcessOpenedCondition::ProcessOpenedCondition(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
}

QDomElement ProcessOpenedCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    QDomElement nameElem = doc->createElement("Name");
    nameElem.appendChild(doc->createTextNode(m_processName));

    elem.appendChild(nameElem);

    return elem;
}

bool ProcessOpenedCondition::privateDeSerialize(QDomElement elem)
{
    QDomElement nameElement;

    //get the process name
    nameElement = elem.firstChildElement("ProcessName");
    if (nameElement.isNull())
    {
        kDebug() << "No processes name specified!  Deserialization failure!";
        return 0;
    }
    m_processName = nameElement.text();

    //connect to the ProcessInfo instance
    ProcessInfo* processInfo = ProcessInfo::instance();

    connect(processInfo, SIGNAL(processAdded(QString)),
            this, SLOT(checkAddedProcess(QString)));
    connect(processInfo, SIGNAL(processRemoved(QString)),
            this, SLOT(checkRemovedProcess(QString)));

    //initialize the condition
    m_openedInstances = processInfo->getRunningProcesses().count(m_processName);

    if (m_openedInstances > 0)
    {
        m_satisfied = true;
	kDebug() << m_processName + " is opened!";
        emit conditionChanged();
    }
    else
    {
        m_satisfied = false;
	kDebug() << m_processName + " is not opened!";
        emit conditionChanged();
    }

    return 1;
}

void ProcessOpenedCondition::checkAddedProcess(QString name)
{
    if (m_processName == name)
    {
        m_openedInstances++;

        if (m_openedInstances == 1)
        {
            m_satisfied = true;
            kDebug() << m_processName + " is opened!";
            emit conditionChanged();
        }
    }
}

void ProcessOpenedCondition::checkRemovedProcess(QString name)
{
    if (m_processName == name)
    {
        m_openedInstances--;

        if (m_openedInstances == 0)
        {
            m_satisfied = false;
            kDebug() << m_processName + " is not opened!";
            emit conditionChanged();
        }
    }
}
