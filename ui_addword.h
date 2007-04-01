/********************************************************************************
** Form generated from reading ui file 'addword.ui'
**
** Created: Tue 20. Mar 12:24:01 2007
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ADDWORD_H
#define UI_ADDWORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_AddWord
{
public:
    QStackedWidget *swMain;
    QWidget *page;
    QWidget *verticalLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QHBoxLayout *hboxLayout1;
    QPushButton *pbBack;
    QPushButton *pbNext;
    QWidget *page_2;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QPushButton *pushButton_4;
    QSlider *horizontalSlider_2;
    QLabel *label_5;
    QGroupBox *groupBox;
    QLabel *label_4;
    QSlider *horizontalSlider;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_5;
    QLabel *label_3;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout2;
    QPushButton *pbBack_2;
    QPushButton *pbNext_2;

    void setupUi(QDialog *AddWord)
    {
    AddWord->setObjectName(QString::fromUtf8("AddWord"));
    AddWord->setWindowIcon(QIcon(QString::fromUtf8(":/images/tray.png")));
    swMain = new QStackedWidget(AddWord);
    swMain->setObjectName(QString::fromUtf8("swMain"));
    swMain->setGeometry(QRect(0, 0, 371, 281));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    verticalLayout = new QWidget(page);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setGeometry(QRect(10, 10, 351, 261));
    vboxLayout = new QVBoxLayout(verticalLayout);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    label = new QLabel(verticalLayout);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    lineEdit = new QLineEdit(verticalLayout);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    hboxLayout->addWidget(lineEdit);


    vboxLayout->addLayout(hboxLayout);

    label_2 = new QLabel(verticalLayout);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setWordWrap(true);

    vboxLayout->addWidget(label_2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    pbBack = new QPushButton(verticalLayout);
    pbBack->setObjectName(QString::fromUtf8("pbBack"));
    pbBack->setIcon(QIcon(QString::fromUtf8(":/images/icons/process-stop.svg")));

    hboxLayout1->addWidget(pbBack);

    pbNext = new QPushButton(verticalLayout);
    pbNext->setObjectName(QString::fromUtf8("pbNext"));
    pbNext->setLayoutDirection(Qt::RightToLeft);
    pbNext->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-next.svg")));

    hboxLayout1->addWidget(pbNext);


    vboxLayout->addLayout(hboxLayout1);

    swMain->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    groupBox_2 = new QGroupBox(page_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 160, 351, 71));
    pushButton_3 = new QPushButton(groupBox_2);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setEnabled(false);
    pushButton_3->setGeometry(QRect(10, 20, 101, 27));
    pushButton_3->setIcon(QIcon(QString::fromUtf8(":/images/icons/media-record.svg")));
    pushButton_6 = new QPushButton(groupBox_2);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setEnabled(false);
    pushButton_6->setGeometry(QRect(240, 20, 101, 27));
    pushButton_6->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-unreadable.svg")));
    pushButton_4 = new QPushButton(groupBox_2);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setEnabled(false);
    pushButton_4->setGeometry(QRect(120, 20, 111, 27));
    pushButton_4->setIcon(QIcon(QString::fromUtf8(":/images/icons/media-playback-start.svg")));
    horizontalSlider_2 = new QSlider(groupBox_2);
    horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
    horizontalSlider_2->setEnabled(false);
    horizontalSlider_2->setGeometry(QRect(10, 50, 231, 16));
    horizontalSlider_2->setMaximum(0);
    horizontalSlider_2->setValue(0);
    horizontalSlider_2->setSliderPosition(0);
    horizontalSlider_2->setOrientation(Qt::Horizontal);
    label_5 = new QLabel(groupBox_2);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setEnabled(false);
    label_5->setGeometry(QRect(271, 50, 68, 20));
    groupBox = new QGroupBox(page_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 80, 351, 71));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(271, 50, 68, 20));
    horizontalSlider = new QSlider(groupBox);
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setGeometry(QRect(10, 50, 231, 16));
    horizontalSlider->setMaximum(7);
    horizontalSlider->setSliderPosition(3);
    horizontalSlider->setOrientation(Qt::Horizontal);
    pushButton_2 = new QPushButton(groupBox);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setGeometry(QRect(10, 20, 101, 27));
    pushButton_2->setIcon(QIcon(QString::fromUtf8(":/images/icons/media-record.svg")));
    pushButton = new QPushButton(groupBox);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(120, 20, 111, 27));
    pushButton->setIcon(QIcon(QString::fromUtf8(":/images/icons/media-playback-start.svg")));
    pushButton_5 = new QPushButton(groupBox);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setGeometry(QRect(240, 20, 101, 27));
    pushButton_5->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-unreadable.svg")));
    label_3 = new QLabel(page_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(10, 20, 341, 51));
    label_3->setWordWrap(true);
    layoutWidget = new QWidget(page_2);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 240, 351, 29));
    hboxLayout2 = new QHBoxLayout(layoutWidget);
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    pbBack_2 = new QPushButton(layoutWidget);
    pbBack_2->setObjectName(QString::fromUtf8("pbBack_2"));
    pbBack_2->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-previous.svg")));

    hboxLayout2->addWidget(pbBack_2);

    pbNext_2 = new QPushButton(layoutWidget);
    pbNext_2->setObjectName(QString::fromUtf8("pbNext_2"));
    pbNext_2->setEnabled(false);
    pbNext_2->setLayoutDirection(Qt::RightToLeft);
    pbNext_2->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-next.svg")));

    hboxLayout2->addWidget(pbNext_2);

    swMain->addWidget(page_2);

    retranslateUi(AddWord);

    QSize size(374, 284);
    size = size.expandedTo(AddWord->minimumSizeHint());
    AddWord->resize(size);

    QObject::connect(pbBack, SIGNAL(clicked()), AddWord, SLOT(reject()));

    swMain->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(AddWord);
    } // setupUi

    void retranslateUi(QDialog *AddWord)
    {
    AddWord->setWindowTitle(QApplication::translate("AddWord", "Wort hinzufuegen", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("AddWord", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Neues Wort:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    lineEdit->setToolTip(QApplication::translate("AddWord", "Der Wert des neuen Wortes (das, das geschrieben werden soll wenn das Wort erkannt wird)", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("AddWord", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sie haben die Funktion: \"Neues Wort hinzuf\303\274gen\" ausgew\303\244hlt.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Diese Schritt-f\303\274r-Schritt Anleitung f\303\274hrt Sie durch diesen Prozess.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0"
        "px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">In diesem Schritt <span style=\" font-weight:600;\">buchstabieren</span> Sie bitte das hinzuzuf\303\274gende Wort.</p></body></html>", 0, QApplication::UnicodeUTF8));
    pbBack->setText(QApplication::translate("AddWord", "  &Abbrechen", 0, QApplication::UnicodeUTF8));
    pbNext->setToolTip(QApplication::translate("AddWord", "Zum n\303\244chsten Schritt", 0, QApplication::UnicodeUTF8));
    pbNext->setText(QApplication::translate("AddWord", "  &Weiter", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("AddWord", "Sample2", 0, QApplication::UnicodeUTF8));
    pushButton_3->setToolTip(QApplication::translate("AddWord", "Startet die Aufnahme", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("AddWord", "Record", 0, QApplication::UnicodeUTF8));
    pushButton_6->setToolTip(QApplication::translate("AddWord", "L\303\266scht die Aufnahme", 0, QApplication::UnicodeUTF8));
    pushButton_6->setText(QApplication::translate("AddWord", "Delete", 0, QApplication::UnicodeUTF8));
    pushButton_4->setToolTip(QApplication::translate("AddWord", "Gibt die Aufnahme wieder (zur Kontrolle)", 0, QApplication::UnicodeUTF8));
    pushButton_4->setText(QApplication::translate("AddWord", "Play", 0, QApplication::UnicodeUTF8));
    horizontalSlider_2->setToolTip(QApplication::translate("AddWord", "Aktuelle position der Wiedergabe", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("AddWord", "0:00 / 0:00", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("AddWord", "Sample1", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("AddWord", "0:03 / 0:07", 0, QApplication::UnicodeUTF8));
    horizontalSlider->setToolTip(QApplication::translate("AddWord", "Aktuelle position der Wiedergabe", 0, QApplication::UnicodeUTF8));
    pushButton_2->setToolTip(QApplication::translate("AddWord", "Startet die Aufnahme", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("AddWord", "Record", 0, QApplication::UnicodeUTF8));
    pushButton->setToolTip(QApplication::translate("AddWord", "Gibt die Aufnahme wieder (zur Kontrolle)", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("AddWord", "Play", 0, QApplication::UnicodeUTF8));
    pushButton_5->setToolTip(QApplication::translate("AddWord", "L\303\266scht die Aufnahme", 0, QApplication::UnicodeUTF8));
    pushButton_5->setText(QApplication::translate("AddWord", "Delete", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("AddWord", "Bitte nehmen Sie das neue Wort nun zweimal auf.\n"
"Sprechen Sie so deutlich wie m\303\266glich und vermeiden sie Umgebungsger\303\244usche.", 0, QApplication::UnicodeUTF8));
    pbBack_2->setToolTip(QApplication::translate("AddWord", "Zur\303\274ck zum eingeben des Wertes der Aufnahme (das, das simon schreibt wenn das Wort erkannt wird)", 0, QApplication::UnicodeUTF8));
    pbBack_2->setText(QApplication::translate("AddWord", "  &Zur\303\274ck", 0, QApplication::UnicodeUTF8));
    pbNext_2->setToolTip(QApplication::translate("AddWord", "Weiter zum n\303\244chsten Schritt (Fertigstellung)", 0, QApplication::UnicodeUTF8));
    pbNext_2->setText(QApplication::translate("AddWord", "  &Weiter", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(AddWord);
    } // retranslateUi

};

namespace Ui {
    class AddWord: public Ui_AddWord {};
} // namespace Ui

#endif // UI_ADDWORD_H
