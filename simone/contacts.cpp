#include "contacts.h"
#include "contactcommand.h"
#include <QStringList>
#include <QDebug>

Contacts::Contacts()
{
    setupRoleNames();
}

void Contacts::setupRoleNames()
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(Qt::UserRole+1, "number");
    setRoleNames(names);
}

QVariant Contacts::data (const QModelIndex & index, int role) const
{
    if (role == Qt::UserRole+1)
        return static_cast<ContactCommand*>(m_commands[index.row()])->number();

    return CommandModel::data(index, role);
}

void Contacts::restore(const QStringList& config)
{
    QList<Command*> commands;
    foreach (const QString& c, config) {
        Command *contact = ContactCommand::deSerialize(c);
        if (!contact)
            qWarning() << "Error serializing " << c;
        else
            commands << contact;
    }

    init(commands);
    reset();
}

void Contacts::removeContact(int index)
{
    if (index > m_commands.count())
            return;

    beginRemoveRows(QModelIndex(), index, index);
    delete m_commands.takeAt(index);
    endRemoveRows();
}

void Contacts::addContact()
{
    int count = m_commands.count();
    beginInsertRows(QModelIndex(), count, count);
    m_commands << new ContactCommand();
    endInsertRows();
}

void Contacts::updateContact(int i, const QString& trigger, const QString& displayName, const QString& number)
{
    if (m_loading)
        return;

    m_commands[i]->setTrigger(trigger);
    static_cast<ContactCommand*>(m_commands[i])->setDisplayName(displayName);
    static_cast<ContactCommand*>(m_commands[i])->setNumber(number);

    QModelIndex lineToChange = index(i, 0);
    if (!lineToChange.isValid()) return;
    emit dataChanged(lineToChange, lineToChange);
}
