#include "condition.h"

Condition::Condition(QObject *parent, const QVariantList &args)
{
    this->setParent(parent);
}

bool Condition::isSatisfied()
{
    if (m_inverted)
    {
        return !m_satisfied;
    }
    else
    {
        return m_satisfied;
    }
}

QDomElement Condition::serialize(QDomDocument *doc)
{
    QDomElement conditionElement = doc->createElement("Condition");

    conditionElement.setAttribute("name", m_pluginName);

    QDomElement invertElem = doc->createElement("Inverted");
    invertElem.appendChild(doc->createTextNode(m_inverted ? "1" : "0"));

    conditionElement.appendChild(invertElem);

    return privateSerialize(doc, conditionElement);
}

bool Condition::deSerialize(QDomElement elem)
{
    m_pluginName = elem.attribute("name");

    QDomElement inverted = elem.firstChildElement();
    m_inverted = (inverted.text().toInt() == 1);

    return this->privateDeSerialize(elem);
}
