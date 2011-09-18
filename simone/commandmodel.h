#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <QAbstractListModel>

#include <QModelIndex>
#include <QList>

class Command;

class CommandModel : public QAbstractListModel
{
protected:
    bool m_loading;
    QList<Command*> m_commands;
    bool init(QList<Command*> commands);

public:
    CommandModel();
    ~CommandModel();
    int columnCount(const QModelIndex& parent = QModelIndex()) const {
        Q_UNUSED(parent);
        return 1;
    }
    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);
        return QModelIndex();
    }
    int rowCount(const QModelIndex &parent) const;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

    void startLoading() { m_loading = true; }
    void stopLoading() { m_loading = false; }

    Command* getCommand(const QString& trigger);
    QStringList store();

    virtual void restore(const QStringList& config)=0;

};

#endif // COMMANDMODEL_H
