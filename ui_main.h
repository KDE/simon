/********************************************************************************
** Form generated from reading ui file 'main.ui'
**
** Created: Tue 20. Mar 12:23:59 2007
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *leSearch;
    QLabel *lbSearch;
    QFrame *line;
    QProgressBar *pbLevel2;
    QProgressBar *pbLevel1;
    QPushButton *pbSettings;
    QPushButton *pbActivision;
    QPushButton *pbClose;
    QPushButton *pbHide;
    QPushButton *pbAddWord;
    QPushButton *pbEditWordList;
    QPushButton *pbTrain;
    QPushButton *pbRunProgram;
    QLabel *lbBg;

    void setupUi(QMainWindow *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    QPalette palette;
    QBrush brush(QColor(130, 191, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush1);
    QBrush brush2(QColor(255, 255, 255, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush2);
    QBrush brush3(QColor(130, 191, 255, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
    QBrush brush4(QColor(255, 255, 255, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush4);
    QBrush brush5(QColor(255, 255, 255, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush5);
    QBrush brush6(QColor(117, 117, 117, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
    QBrush brush7(QColor(255, 255, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush7);
    QBrush brush8(QColor(255, 255, 255, 255));
    brush8.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush8);
    MainWindow->setPalette(palette);
    MainWindow->setWindowIcon(QIcon(QString::fromUtf8("../../../../../../../:/images/tray.png")));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    leSearch = new QLineEdit(centralwidget);
    leSearch->setObjectName(QString::fromUtf8("leSearch"));
    leSearch->setGeometry(QRect(210, 40, 161, 25));
    lbSearch = new QLabel(centralwidget);
    lbSearch->setObjectName(QString::fromUtf8("lbSearch"));
    lbSearch->setGeometry(QRect(70, 40, 91, 17));
    QPalette palette1;
    QBrush brush9(QColor(175, 190, 200, 255));
    brush9.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::WindowText, brush9);
    QBrush brush10(QColor(175, 190, 200, 255));
    brush10.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::Text, brush10);
    QBrush brush11(QColor(175, 190, 200, 255));
    brush11.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
    QBrush brush12(QColor(175, 190, 200, 255));
    brush12.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Inactive, QPalette::Text, brush12);
    QBrush brush13(QColor(117, 117, 117, 255));
    brush13.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
    QBrush brush14(QColor(117, 117, 117, 255));
    brush14.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Disabled, QPalette::Text, brush14);
    lbSearch->setPalette(palette1);
    line = new QFrame(centralwidget);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(0, 550, 1001, 16));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    pbLevel2 = new QProgressBar(centralwidget);
    pbLevel2->setObjectName(QString::fromUtf8("pbLevel2"));
    pbLevel2->setGeometry(QRect(970, 10, 20, 541));
    pbLevel2->setMinimum(0);
    pbLevel2->setMaximum(32768);
    pbLevel2->setValue(0);
    pbLevel2->setTextVisible(false);
    pbLevel2->setOrientation(Qt::Vertical);
    pbLevel1 = new QProgressBar(centralwidget);
    pbLevel1->setObjectName(QString::fromUtf8("pbLevel1"));
    pbLevel1->setGeometry(QRect(10, 10, 20, 541));
    pbLevel1->setMinimum(0);
    pbLevel1->setMaximum(32768);
    pbLevel1->setValue(0);
    pbLevel1->setTextVisible(false);
    pbLevel1->setOrientation(Qt::Vertical);
    pbSettings = new QPushButton(centralwidget);
    pbSettings->setObjectName(QString::fromUtf8("pbSettings"));
    pbSettings->setGeometry(QRect(10, 570, 221, 51));
    pbSettings->setIcon(QIcon(QString::fromUtf8(":/images/icons/computer.svg")));
    pbSettings->setIconSize(QSize(32, 32));
    pbActivision = new QPushButton(centralwidget);
    pbActivision->setObjectName(QString::fromUtf8("pbActivision"));
    pbActivision->setGeometry(QRect(280, 570, 201, 51));
    pbActivision->setIcon(QIcon(QString::fromUtf8(":/images/icons/media-playback-pause.svg")));
    pbActivision->setIconSize(QSize(32, 32));
    pbClose = new QPushButton(centralwidget);
    pbClose->setObjectName(QString::fromUtf8("pbClose"));
    pbClose->setGeometry(QRect(490, 570, 201, 51));
    pbClose->setIcon(QIcon(QString::fromUtf8(":/images/icons/system-log-out.svg")));
    pbClose->setIconSize(QSize(32, 32));
    pbHide = new QPushButton(centralwidget);
    pbHide->setObjectName(QString::fromUtf8("pbHide"));
    pbHide->setGeometry(QRect(740, 570, 251, 51));
    pbHide->setIcon(QIcon(QString::fromUtf8(":/images/icons/window-new.svg")));
    pbHide->setIconSize(QSize(32, 32));
    pbAddWord = new QPushButton(centralwidget);
    pbAddWord->setObjectName(QString::fromUtf8("pbAddWord"));
    pbAddWord->setGeometry(QRect(110, 170, 381, 121));
    QPalette palette2;
    QBrush brush15(QColor(175, 190, 200, 255));
    brush15.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush15);
    QBrush brush16(QColor(175, 190, 200, 255));
    brush16.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush16);
    QBrush brush17(QColor(119, 119, 119, 255));
    brush17.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush17);
    pbAddWord->setPalette(palette2);
    QFont font;
    font.setPointSize(20);
    pbAddWord->setFont(font);
    pbAddWord->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-new.svg")));
    pbAddWord->setIconSize(QSize(56, 56));
    pbAddWord->setFlat(true);
    pbEditWordList = new QPushButton(centralwidget);
    pbEditWordList->setObjectName(QString::fromUtf8("pbEditWordList"));
    pbEditWordList->setGeometry(QRect(110, 350, 381, 121));
    QPalette palette3;
    QBrush brush18(QColor(255, 255, 255, 255));
    brush18.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Active, QPalette::WindowText, brush18);
    QBrush brush19(QColor(255, 255, 255, 255));
    brush19.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Active, QPalette::Text, brush19);
    QBrush brush20(QColor(175, 190, 200, 255));
    brush20.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush20);
    QBrush brush21(QColor(255, 255, 255, 255));
    brush21.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush21);
    QBrush brush22(QColor(255, 255, 255, 255));
    brush22.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Inactive, QPalette::Text, brush22);
    QBrush brush23(QColor(175, 190, 200, 255));
    brush23.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush23);
    QBrush brush24(QColor(255, 255, 255, 255));
    brush24.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush24);
    QBrush brush25(QColor(255, 255, 255, 255));
    brush25.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Disabled, QPalette::Text, brush25);
    QBrush brush26(QColor(255, 255, 255, 255));
    brush26.setStyle(Qt::SolidPattern);
    palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush26);
    pbEditWordList->setPalette(palette3);
    QFont font1;
    font1.setPointSize(20);
    font1.setKerning(true);
    font1.setStyleStrategy(QFont::PreferDefault);
    pbEditWordList->setFont(font1);
    pbEditWordList->setIcon(QIcon(QString::fromUtf8(":/images/icons/format-justify-fill.svg")));
    pbEditWordList->setIconSize(QSize(56, 56));
    pbEditWordList->setFlat(true);
    pbTrain = new QPushButton(centralwidget);
    pbTrain->setObjectName(QString::fromUtf8("pbTrain"));
    pbTrain->setGeometry(QRect(570, 170, 351, 121));
    QPalette palette4;
    QBrush brush27(QColor(175, 190, 200, 255));
    brush27.setStyle(Qt::SolidPattern);
    palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush27);
    QBrush brush28(QColor(175, 190, 200, 255));
    brush28.setStyle(Qt::SolidPattern);
    palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush28);
    QBrush brush29(QColor(119, 119, 119, 255));
    brush29.setStyle(Qt::SolidPattern);
    palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush29);
    pbTrain->setPalette(palette4);
    QFont font2;
    font2.setPointSize(20);
    pbTrain->setFont(font2);
    pbTrain->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-properties.svg")));
    pbTrain->setIconSize(QSize(56, 56));
    pbTrain->setFlat(true);
    pbRunProgram = new QPushButton(centralwidget);
    pbRunProgram->setObjectName(QString::fromUtf8("pbRunProgram"));
    pbRunProgram->setGeometry(QRect(570, 350, 351, 121));
    QPalette palette5;
    QBrush brush30(QColor(175, 190, 200, 255));
    brush30.setStyle(Qt::SolidPattern);
    palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush30);
    QBrush brush31(QColor(175, 190, 200, 255));
    brush31.setStyle(Qt::SolidPattern);
    palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush31);
    QBrush brush32(QColor(119, 119, 119, 255));
    brush32.setStyle(Qt::SolidPattern);
    palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush32);
    pbRunProgram->setPalette(palette5);
    QFont font3;
    font3.setPointSize(20);
    pbRunProgram->setFont(font3);
    pbRunProgram->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-system.svg")));
    pbRunProgram->setIconSize(QSize(56, 56));
    pbRunProgram->setFlat(true);
    lbBg = new QLabel(centralwidget);
    lbBg->setObjectName(QString::fromUtf8("lbBg"));
    lbBg->setGeometry(QRect(0, 0, 1001, 557));
    MainWindow->setCentralWidget(centralwidget);
    lbSearch->setBuddy(leSearch);
    QWidget::setTabOrder(leSearch, pbAddWord);
    QWidget::setTabOrder(pbAddWord, pbTrain);
    QWidget::setTabOrder(pbTrain, pbEditWordList);
    QWidget::setTabOrder(pbEditWordList, pbRunProgram);
    QWidget::setTabOrder(pbRunProgram, pbSettings);
    QWidget::setTabOrder(pbSettings, pbActivision);
    QWidget::setTabOrder(pbActivision, pbClose);
    QWidget::setTabOrder(pbClose, pbHide);

    retranslateUi(MainWindow);

    QSize size(999, 631);
    size = size.expandedTo(MainWindow->minimumSizeHint());
    MainWindow->resize(size);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "simon", 0, QApplication::UnicodeUTF8));
    lbSearch->setText(QApplication::translate("MainWindow", "&Suchen nach:", 0, QApplication::UnicodeUTF8));
    pbSettings->setToolTip(QApplication::translate("MainWindow", "Einstellungen", 0, QApplication::UnicodeUTF8));
    pbSettings->setText(QApplication::translate("MainWindow", "  &System", 0, QApplication::UnicodeUTF8));
    pbActivision->setToolTip(QApplication::translate("MainWindow", "De-/aktiviert das System", 0, QApplication::UnicodeUTF8));
    pbActivision->setText(QApplication::translate("MainWindow", "  &Deaktivieren", 0, QApplication::UnicodeUTF8));
    pbClose->setToolTip(QApplication::translate("MainWindow", "Beendet simon", 0, QApplication::UnicodeUTF8));
    pbClose->setText(QApplication::translate("MainWindow", "  &Beenden", 0, QApplication::UnicodeUTF8));
    pbHide->setToolTip(QApplication::translate("MainWindow", "Minimiert das Fenster in den Systray (Bereich neben der Uhr)", 0, QApplication::UnicodeUTF8));
    pbHide->setText(QApplication::translate("MainWindow", "  &Verstecken", 0, QApplication::UnicodeUTF8));
    pbAddWord->setToolTip(QApplication::translate("MainWindow", "F\303\274gt ein neues Wort in das Sprachmodell ein", 0, QApplication::UnicodeUTF8));
    pbAddWord->setText(QApplication::translate("MainWindow", "  Wort &hinzuf\303\274gen", 0, QApplication::UnicodeUTF8));
    pbEditWordList->setToolTip(QApplication::translate("MainWindow", "Vokabel durchsehen / editieren und \303\244ndern. Au\303\237erdem kann hier ein per-Wort Trainingsprogramm zusammengestellt werden.", 0, QApplication::UnicodeUTF8));
    pbEditWordList->setStyleSheet(QString());
    pbEditWordList->setText(QApplication::translate("MainWindow", "  &Wortliste", 0, QApplication::UnicodeUTF8));
    pbTrain->setToolTip(QApplication::translate("MainWindow", "Hier k\303\266nnen Sie das Sprachmodell editieren und somit die Erkennungsrate erh\303\266hen", 0, QApplication::UnicodeUTF8));
    pbTrain->setText(QApplication::translate("MainWindow", "  &Trainieren", 0, QApplication::UnicodeUTF8));
    pbRunProgram->setToolTip(QApplication::translate("MainWindow", "Zeigt den Ausf\303\274hrendialog an. Hier k\303\266nnen Sie Kommandos ausf\303\274hren und \303\244ndern", 0, QApplication::UnicodeUTF8));
    pbRunProgram->setText(QApplication::translate("MainWindow", "  &Ausf\303\274hren", 0, QApplication::UnicodeUTF8));
    lbBg->setText(QString());
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAIN_H
