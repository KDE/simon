/********************************************************************************
** Form generated from reading ui file 'trainmain.ui'
**
** Created: Sun Apr 1 21:29:14 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRAINMAIN_H
#define UI_TRAINMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

class Ui_TrainMain
{
public:
    QPushButton *pbImportText;
    QPushButton *pbDelText;
    QPushButton *pbBack;
    QPushButton *pbWordList;
    QPushButton *pbTrainText;
    QTableWidget *twTrainingWords;

    void setupUi(QDialog *TrainMain)
    {
    TrainMain->setObjectName(QString::fromUtf8("TrainMain"));
    TrainMain->setWindowIcon(QIcon(QString::fromUtf8(":/images/tray.png")));
    pbImportText = new QPushButton(TrainMain);
    pbImportText->setObjectName(QString::fromUtf8("pbImportText"));
    pbImportText->setGeometry(QRect(10, 410, 211, 31));
    pbImportText->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-open.svg")));
    pbDelText = new QPushButton(TrainMain);
    pbDelText->setObjectName(QString::fromUtf8("pbDelText"));
    pbDelText->setGeometry(QRect(240, 410, 211, 31));
    pbDelText->setIcon(QIcon(QString::fromUtf8(":/images/icons/process-stop.svg")));
    pbBack = new QPushButton(TrainMain);
    pbBack->setObjectName(QString::fromUtf8("pbBack"));
    pbBack->setGeometry(QRect(10, 450, 211, 31));
    pbBack->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-previous.svg")));
    pbWordList = new QPushButton(TrainMain);
    pbWordList->setObjectName(QString::fromUtf8("pbWordList"));
    pbWordList->setGeometry(QRect(240, 450, 211, 31));
    pbWordList->setIcon(QIcon(QString::fromUtf8(":/images/icons/format-justify-fill.svg")));
    pbTrainText = new QPushButton(TrainMain);
    pbTrainText->setObjectName(QString::fromUtf8("pbTrainText"));
    pbTrainText->setGeometry(QRect(10, 360, 441, 41));
    pbTrainText->setLayoutDirection(Qt::RightToLeft);
    pbTrainText->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-next.svg")));
    pbTrainText->setIconSize(QSize(20, 20));
    twTrainingWords = new QTableWidget(TrainMain);
    twTrainingWords->setObjectName(QString::fromUtf8("twTrainingWords"));
    twTrainingWords->setGeometry(QRect(10, 10, 441, 341));

    retranslateUi(TrainMain);

    QSize size(459, 485);
    size = size.expandedTo(TrainMain->minimumSizeHint());
    TrainMain->resize(size);

    QObject::connect(pbBack, SIGNAL(clicked()), TrainMain, SLOT(reject()));

    QMetaObject::connectSlotsByName(TrainMain);
    } // setupUi

    void retranslateUi(QDialog *TrainMain)
    {
    TrainMain->setWindowTitle(QApplication::translate("TrainMain", "Training", 0, QApplication::UnicodeUTF8));
    pbImportText->setToolTip(QApplication::translate("TrainMain", "Importiert neue Texte aus dem Internet oder aus Dateien", 0, QApplication::UnicodeUTF8));
    pbImportText->setText(QApplication::translate("TrainMain", "  Text(e) &importieren", 0, QApplication::UnicodeUTF8));
    pbDelText->setToolTip(QApplication::translate("TrainMain", "L\303\266schen des markierten Textes aus dem System", 0, QApplication::UnicodeUTF8));
    pbDelText->setText(QApplication::translate("TrainMain", "  Markierten Text &l\303\266schen", 0, QApplication::UnicodeUTF8));
    pbBack->setToolTip(QApplication::translate("TrainMain", "Schlie\303\237t den Dialog", 0, QApplication::UnicodeUTF8));
    pbBack->setText(QApplication::translate("TrainMain", "  &Zur\303\274ck", 0, QApplication::UnicodeUTF8));
    pbWordList->setToolTip(QApplication::translate("TrainMain", "Schlie\303\237t den Dialog und \303\266ffnet die Wordliste - Dort kann ein individuelles Trainingsprogramm erstellt werden", 0, QApplication::UnicodeUTF8));
    pbWordList->setText(QApplication::translate("TrainMain", "  Wechsle zur &Wortliste", 0, QApplication::UnicodeUTF8));
    pbTrainText->setToolTip(QApplication::translate("TrainMain", "Startet das Training des selektierten Textes.", 0, QApplication::UnicodeUTF8));
    pbTrainText->setText(QApplication::translate("TrainMain", "                    (Markierten Text &vorlesen (trainieren", 0, QApplication::UnicodeUTF8));
    if (twTrainingWords->columnCount() < 3)
        twTrainingWords->setColumnCount(3);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("TrainMain", "Name", 0, QApplication::UnicodeUTF8));
    twTrainingWords->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("TrainMain", "Seiten", 0, QApplication::UnicodeUTF8));
    twTrainingWords->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("TrainMain", "Relevanz", 0, QApplication::UnicodeUTF8));
    twTrainingWords->setHorizontalHeaderItem(2, __colItem2);
    Q_UNUSED(TrainMain);
    } // retranslateUi

};

namespace Ui {
    class TrainMain: public Ui_TrainMain {};
} // namespace Ui

#endif // UI_TRAINMAIN_H
