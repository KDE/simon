/********************************************************************************
** Form generated from reading ui file 'rundialog.ui'
**
** Created: Tue 20. Mar 12:24:00 2007
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_RUNDIALOG_H
#define UI_RUNDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

class Ui_RunDialog
{
public:
    QLabel *lbSearch;
    QLabel *lbShow;
    QTableWidget *twPrograms;
    QPushButton *pbBack;
    QPushButton *pbRun;
    QLineEdit *leSearch;
    QPushButton *pbClearSearch;
    QComboBox *cbShow;

    void setupUi(QDialog *RunDialog)
    {
    RunDialog->setObjectName(QString::fromUtf8("RunDialog"));
    RunDialog->setWindowIcon(QIcon(QString::fromUtf8(":/images/tray.png")));
    lbSearch = new QLabel(RunDialog);
    lbSearch->setObjectName(QString::fromUtf8("lbSearch"));
    lbSearch->setGeometry(QRect(10, 13, 59, 17));
    lbShow = new QLabel(RunDialog);
    lbShow->setObjectName(QString::fromUtf8("lbShow"));
    lbShow->setGeometry(QRect(240, 10, 61, 17));
    twPrograms = new QTableWidget(RunDialog);
    twPrograms->setObjectName(QString::fromUtf8("twPrograms"));
    twPrograms->setGeometry(QRect(10, 50, 441, 381));
    twPrograms->setAlternatingRowColors(true);
    twPrograms->setSelectionMode(QAbstractItemView::SingleSelection);
    twPrograms->setSelectionBehavior(QAbstractItemView::SelectRows);
    twPrograms->setShowGrid(false);
    twPrograms->setGridStyle(Qt::SolidLine);
    twPrograms->setSortingEnabled(false);
    pbBack = new QPushButton(RunDialog);
    pbBack->setObjectName(QString::fromUtf8("pbBack"));
    pbBack->setGeometry(QRect(10, 440, 91, 27));
    pbBack->setLayoutDirection(Qt::LeftToRight);
    pbBack->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-previous.svg")));
    pbBack->setCheckable(false);
    pbBack->setChecked(false);
    pbRun = new QPushButton(RunDialog);
    pbRun->setObjectName(QString::fromUtf8("pbRun"));
    pbRun->setGeometry(QRect(350, 440, 101, 27));
    pbRun->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-system.svg")));
    pbRun->setDefault(true);
    pbRun->setFlat(false);
    leSearch = new QLineEdit(RunDialog);
    leSearch->setObjectName(QString::fromUtf8("leSearch"));
    leSearch->setGeometry(QRect(70, 10, 113, 25));
    pbClearSearch = new QPushButton(RunDialog);
    pbClearSearch->setObjectName(QString::fromUtf8("pbClearSearch"));
    pbClearSearch->setGeometry(QRect(190, 10, 21, 27));
    cbShow = new QComboBox(RunDialog);
    cbShow->setObjectName(QString::fromUtf8("cbShow"));
    cbShow->setGeometry(QRect(310, 10, 142, 22));
    lbSearch->setBuddy(leSearch);
    lbShow->setBuddy(cbShow);

    retranslateUi(RunDialog);

    QSize size(460, 476);
    size = size.expandedTo(RunDialog->minimumSizeHint());
    RunDialog->resize(size);

    QObject::connect(pbBack, SIGNAL(clicked()), RunDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(RunDialog);
    } // setupUi

    void retranslateUi(QDialog *RunDialog)
    {
    RunDialog->setWindowTitle(QApplication::translate("RunDialog", "Ausf\303\274hren", 0, QApplication::UnicodeUTF8));
    lbSearch->setText(QApplication::translate("RunDialog", "&Suchen:", 0, QApplication::UnicodeUTF8));
    lbShow->setText(QApplication::translate("RunDialog", "Anzei&gen:", 0, QApplication::UnicodeUTF8));
    if (twPrograms->columnCount() < 3)
        twPrograms->setColumnCount(3);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("RunDialog", "Name", 0, QApplication::UnicodeUTF8));
    twPrograms->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("RunDialog", "Typ", 0, QApplication::UnicodeUTF8));
    twPrograms->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("RunDialog", "Wert", 0, QApplication::UnicodeUTF8));
    twPrograms->setHorizontalHeaderItem(2, __colItem2);
    pbBack->setToolTip(QApplication::translate("RunDialog", "Zur\303\274ck zum Hauptmen\303\274", 0, QApplication::UnicodeUTF8));
    pbBack->setText(QApplication::translate("RunDialog", "&Zur\303\274ck", 0, QApplication::UnicodeUTF8));
    pbRun->setToolTip(QApplication::translate("RunDialog", "F\303\274hrt das selektierte Kommando aus", 0, QApplication::UnicodeUTF8));
    pbRun->setText(QApplication::translate("RunDialog", "&Ausf\303\274hren", 0, QApplication::UnicodeUTF8));
    leSearch->setToolTip(QApplication::translate("RunDialog", "Schr\303\244nkt die Liste auf die Eingabe ein", 0, QApplication::UnicodeUTF8));
    pbClearSearch->setToolTip(QApplication::translate("RunDialog", "Zur\303\274cksetzen der Suche", 0, QApplication::UnicodeUTF8));
    pbClearSearch->setText(QApplication::translate("RunDialog", "&x", 0, QApplication::UnicodeUTF8));
    cbShow->clear();
    cbShow->addItem(QIcon(QString::fromUtf8(":/images/icons/system-file-manager.svg")), QApplication::translate("RunDialog", "Alles", 0, QApplication::UnicodeUTF8));
    cbShow->addItem(QIcon(QString::fromUtf8(":/images/icons/emblem-system.svg")), QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
    cbShow->addItem(QIcon(QString::fromUtf8(":/images/icons/folder.svg")), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
    cbShow->addItem(QIcon(QString::fromUtf8(":/images/icons/format-text-bold.svg")), QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
    cbShow->setToolTip(QApplication::translate("RunDialog", "Schr\303\244nkt die Ausgabe auf gewisse Gruppen ein", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(RunDialog);
    } // retranslateUi

};

namespace Ui {
    class RunDialog: public Ui_RunDialog {};
} // namespace Ui

#endif // UI_RUNDIALOG_H
