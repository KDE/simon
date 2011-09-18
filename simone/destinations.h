#ifndef DESTINATIONS_H
#define DESTINATIONS_H

#include "commandmodel.h"

class QStringList;
class Destinations : public CommandModel
{
Q_OBJECT

private:
    void setupRoleNames();

public:
    Destinations();
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

    void restore(const QStringList& config);

public slots:
    void removeDestination(int index);
    void addDestination();
    void updateDestination(int index, const QString& trigger, const QString& destination);
};

#endif // CONTACTS_H
