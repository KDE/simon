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
#ifdef __WIN32
    QListView *iconView;
    QStandardItemModel *model;
#endif
    QString resourceIdStr;
    QLineEdit *leFromFile;
    QPushButton *pbFromFile;
    
public:
    IconDialog(QWidget *parent = 0);
    ~IconDialog(){}

    void showIcons(QString currentIcon = "");
    QString getIcon(){return resourceIdStr;}

private slots:
    void safeIconInformation();
#ifdef __WIN32
    void enableSystemIcons();
    void enableLineEdit();
#endif
    void openFileDialog();

signals:
    void iconSelected(QString resourceId);
};

#endif
