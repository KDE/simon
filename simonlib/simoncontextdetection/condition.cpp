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

void Condition::serialize(QDomDocument *doc)
{

}

void Condition::deSerialize(QDomElement elem)
{
    QDomElement inverted = elem.firstChildElement();
    m_inverted = (inverted.text().toInt() == 1);

    this->privateDeSerialize(elem);
}
