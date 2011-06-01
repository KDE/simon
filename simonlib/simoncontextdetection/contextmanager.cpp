#include "contextmanager.h"
#include "KDE/KService"
#include "KDebug"
#include <QTextStream>

ContextManager* ContextManager::m_instance;

//testing
#include <QFile>
//end testing

ContextManager::ContextManager(QObject *parent) :
    QObject(parent)
{

}

void ContextManager::test()
{
    //testing
    QFile testXml;
    testXml.setFileName("/home/anash/Documents/QtProjects/ProcessListing-build-desktop/test.xml");
    testXml.open(QFile::ReadWrite);
    QDomDocument doc;
    doc.setContent(&testXml);
    testXml.close();

    QDomElement elem;
    elem = doc.documentElement();

    CompoundCondition *cCondition;
    cCondition = CompoundCondition::createInstance(elem);

    //serialize the compound condition and print the result
    QString str;
    QTextStream stream(&str);
    QDomDocument *parentDoc = new QDomDocument();
    elem = cCondition->serialize(parentDoc);
    elem.save(stream, 4);

    kDebug() << "Compound Condition: \n" + str;

    //end testing
}

ContextManager::~ContextManager()
{
    qDeleteAll(m_conditions);
}

ContextManager* ContextManager::instance()
{
    if (!m_instance)
        m_instance = new ContextManager();

    return m_instance;
}

Condition* ContextManager::getCondition(const QDomElement &elem)
{
    Condition* condition;
    QString source;
    QString str;
    QTextStream stream(&str);
    
    //check to see if the condition has already been created
    //if so, just return the existing condition
    elem.save(stream, 4);
    kDebug() << "Condition: " + str;
    condition = m_conditionLookup.value(str, NULL);
    if (condition != NULL)
    {
	kDebug() << "Condition is a duplicate!";
	return condition;
    }

    //get the name of the service
    source = elem.attribute("name");

    //get the service
    KService::Ptr service = KService::serviceByStorageId(source);
    if (!service) {
      kDebug() << "Service not found! Source: " << source;
      return 0;
    }

    //create the factory for the service
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
    if (factory) {
      condition = factory->create<Condition>();
      factory->deleteLater();
    }
    else {
      kDebug() << "Factory not found! Source: " << source;
      return 0;
    }

    //deserialize the service data
    condition->deSerialize(elem);
    
    //add the condition to member containers for future lookup
    m_conditions.push_back(condition);
    m_conditionLookup.insert(str, condition);

    return condition;
}
