 
#ifndef UI_SAMPLESHARE_H
#define UI_SAMPLESHARE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kdialog.h"

QT_BEGIN_NAMESPACE

class Ui_sampleShare
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    KDialog *kdialog;

    void setupUi(QWidget *sampleShare)
    {
        if (sampleShare->objectName().isEmpty())
            sampleShare->setObjectName(QString::fromUtf8("sampleShare"));
        sampleShare->resize(175, 89);
        verticalLayout = new QVBoxLayout(sampleShare);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(sampleShare);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        kdialog = new KDialog(sampleShare);
        kdialog->setObjectName(QString::fromUtf8("kdialog"));

        verticalLayout->addWidget(kdialog);


        retranslateUi(sampleShare);

        QMetaObject::connectSlotsByName(sampleShare);
    } // setupUi

    void retranslateUi(QWidget *sampleShare)
    {
        sampleShare->setWindowTitle(QApplication::translate("sampleShare", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("sampleShare", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Hello World</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sampleShare: public Ui_sampleShare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLESHARE_H
