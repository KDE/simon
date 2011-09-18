#ifndef CONTACTS_H
#define CONTACTS_H

#include "commandmodel.h"

class QStringList;
class Contacts : public CommandModel
{
Q_OBJECT

private:
    void setupRoleNames();

    void load(const QStringList& list);

public:
    Contacts();
    Contacts(const Contacts& other); //deep copy
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

    void restore(const QStringList& config);
    QStringList store();

public slots:
    void removeContact(int index);
    void addContact();
    void updateContact(int index, const QString& trigger, const QString& displayName, const QString& number);
};

#endif // CONTACTS_H
