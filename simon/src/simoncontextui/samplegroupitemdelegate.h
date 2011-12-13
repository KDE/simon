#ifndef SAMPLEGROUPITEMDELEGATE_H
#define SAMPLEGROUPITEMDELEGATE_H

#include <QStyledItemDelegate>

class SampleGroupItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SampleGroupItemDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

public slots:

};

#endif // SAMPLEGROUPITEMDELEGATE_H
