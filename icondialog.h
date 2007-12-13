//
// C++ Interface: icondialog
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ICONDIALOG_H
#define  ICONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>

class QListView;
class QIcon;
class QLineEdit;
class QPushButton;

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class IconDialog : public QDialog
{
    Q_OBJECT


private:
    QListView *iconView;
    QStandardItemModel *model;
    QString resourceIdStr;
    QLineEdit *leFromFile;
    QPushButton *pbFromFile;
    
public:
    IconDialog(QWidget *parent = 0);
    ~IconDialog(){}

    void showIcons(QString currentIcon = "");
    QString getIcon(){return resourceIdStr;}
    //QIcon loadIcon(QString file, int resourceId);

private slots:
    void safeIconInformation();
    void enableSystemIcons();
    void enableLineEdit();
    void openFileDialog();

signals:
    void iconSelected(QString resourceId);
};

#endif
