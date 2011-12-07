#ifndef SAMPLEGROUPCONDITIONMODEL_H
#define SAMPLEGROUPCONDITIONMODEL_H

#include <QAbstractItemModel>
#include "simoncontextdetection_export.h"
#include "contextmanager.h"

class SIMONCONTEXTDETECTION_EXPORT SampleGroupConditionModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SampleGroupConditionModel(QObject *parent = 0);

    /**
   *
   * \return The index of the item in the model with the given \var row, \var column, and \var parent.
   */
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

    /**
   *
   * \return The item flags for the given \var index
   */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
   *
   * \return The data for the given \var role in the header with the specified \var orientation.
   */
    QVariant headerData(int, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /**
   *
   * \return The index of the parent of the item with the given \var index
   */
    QModelIndex parent(const QModelIndex &index) const;

    /**
   *
   * \return The number of rows under the given \var parent.
   */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

//    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
//    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    /**
   *
   * \return The data stored under the given \var role for the item referred to by \var index.
   */
    virtual QVariant data(const QModelIndex &index, int role) const;


    /**
   *
   * \return The data stored under the given \var role for the item referred to by \var index.
   */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /**
   *
   * \return The number of columns under the given \var parent.
   */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

public slots:
    /**
   * \brief Updates the model to be in sync with ContextManager
   *
   */
    void update();

};

#endif // SAMPLEGROUPCONDITIONMODEL_H
