/********************************************************************************
** Form generated from reading ui file 'settings.ui'
**
** Created: Tue 20. Mar 12:24:01 2007
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

class Ui_SettingsDialog
{
public:
    QFrame *frame;
    QPushButton *pbRevert;
    QLabel *lbSystemSettings;
    QPushButton *pbSystemSettings;
    QLabel *lbSoundSettings;
    QPushButton *pbSoundSettings;
    QLabel *lbCommands;
    QPushButton *pbCommands;
    QPushButton *pbProtocol;
    QLabel *lbProtocol;
    QLabel *lbRevert;
    QStackedWidget *swSettings;
    QWidget *page;
    QWidget *gridLayout;
    QGridLayout *gridLayout1;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QLineEdit *lePrompts;
    QSpacerItem *spacerItem3;
    QSpacerItem *spacerItem4;
    QSpacerItem *spacerItem5;
    QSpacerItem *spacerItem6;
    QSpacerItem *spacerItem7;
    QSpacerItem *spacerItem8;
    QSpacerItem *spacerItem9;
    QLineEdit *leVocab;
    QSpacerItem *spacerItem10;
    QCheckBox *cbStartSimonOnBoot;
    QSpacerItem *spacerItem11;
    QSpacerItem *spacerItem12;
    QSpacerItem *spacerItem13;
    QSpacerItem *spacerItem14;
    QSpacerItem *spacerItem15;
    QLabel *lbJuliusdAddr;
    QLabel *lbGrammar;
    QSpacerItem *spacerItem16;
    QLabel *lbFiles;
    QSpacerItem *spacerItem17;
    QSpacerItem *spacerItem18;
    QLabel *lbConfig;
    QSpacerItem *spacerItem19;
    QSpacerItem *spacerItem20;
    QSpacerItem *spacerItem21;
    QSpacerItem *spacerItem22;
    QLabel *lbAutostart;
    QSpacerItem *spacerItem23;
    QLineEdit *leGrammar;
    QSpacerItem *spacerItem24;
    QLineEdit *leConfig;
    QLineEdit *leCommands;
    QSpacerItem *spacerItem25;
    QSpacerItem *spacerItem26;
    QSpacerItem *spacerItem27;
    QLabel *lbVocab;
    QSpacerItem *spacerItem28;
    QSpacerItem *spacerItem29;
    QSpacerItem *spacerItem30;
    QCheckBox *cbStartJuliusdOnBoot;
    QLabel *lbLexicon;
    QCheckBox *cbStartJuliusAsNeeded;
    QSpacerItem *spacerItem31;
    QSpacerItem *spacerItem32;
    QSpinBox *sbPort;
    QLabel *lbJuliusdPort;
    QLabel *label;
    QLineEdit *leAddress;
    QLabel *lbPrompts;
    QSpacerItem *spacerItem33;
    QSpacerItem *spacerItem34;
    QLineEdit *leLexicon;
    QLabel *lbCommands1;
    QLabel *lbJuliusd;
    QSpacerItem *spacerItem35;
    QSpacerItem *spacerItem36;
    QSpacerItem *spacerItem37;
    QSpacerItem *spacerItem38;
    QSpacerItem *spacerItem39;
    QSpacerItem *spacerItem40;
    QCheckBox *checkBox;
    QFrame *liSep1;
    QLabel *lbHelpSystem;
    QWidget *page_2;
    QPushButton *pbConfirm;
    QPushButton *pbApply;
    QPushButton *pbBack;

    void setupUi(QDialog *SettingsDialog)
    {
    SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
    SettingsDialog->setWindowIcon(QIcon(QString::fromUtf8(":/images/tray.png")));
    frame = new QFrame(SettingsDialog);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setGeometry(QRect(10, 10, 981, 111));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    pbRevert = new QPushButton(frame);
    pbRevert->setObjectName(QString::fromUtf8("pbRevert"));
    pbRevert->setGeometry(QRect(820, 70, 141, 31));
    pbRevert->setFlat(true);
    lbSystemSettings = new QLabel(frame);
    lbSystemSettings->setObjectName(QString::fromUtf8("lbSystemSettings"));
    lbSystemSettings->setGeometry(QRect(60, 10, 51, 61));
    lbSystemSettings->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/computer.svg")));
    lbSystemSettings->setScaledContents(false);
    pbSystemSettings = new QPushButton(frame);
    pbSystemSettings->setObjectName(QString::fromUtf8("pbSystemSettings"));
    pbSystemSettings->setGeometry(QRect(10, 70, 141, 31));
    pbSystemSettings->setCheckable(true);
    pbSystemSettings->setChecked(true);
    pbSystemSettings->setFlat(true);
    lbSoundSettings = new QLabel(frame);
    lbSoundSettings->setObjectName(QString::fromUtf8("lbSoundSettings"));
    lbSoundSettings->setGeometry(QRect(280, 10, 51, 61));
    lbSoundSettings->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/gnome-settings-sound.svg")));
    lbSoundSettings->setScaledContents(false);
    pbSoundSettings = new QPushButton(frame);
    pbSoundSettings->setObjectName(QString::fromUtf8("pbSoundSettings"));
    pbSoundSettings->setGeometry(QRect(230, 70, 141, 31));
    pbSoundSettings->setFlat(true);
    lbCommands = new QLabel(frame);
    lbCommands->setObjectName(QString::fromUtf8("lbCommands"));
    lbCommands->setGeometry(QRect(480, 10, 51, 61));
    lbCommands->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/terminal.svg")));
    lbCommands->setScaledContents(false);
    pbCommands = new QPushButton(frame);
    pbCommands->setObjectName(QString::fromUtf8("pbCommands"));
    pbCommands->setGeometry(QRect(430, 70, 141, 31));
    pbCommands->setFlat(true);
    pbProtocol = new QPushButton(frame);
    pbProtocol->setObjectName(QString::fromUtf8("pbProtocol"));
    pbProtocol->setGeometry(QRect(630, 70, 141, 31));
    pbProtocol->setFlat(true);
    lbProtocol = new QLabel(frame);
    lbProtocol->setObjectName(QString::fromUtf8("lbProtocol"));
    lbProtocol->setGeometry(QRect(680, 10, 51, 61));
    lbProtocol->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/text-editor.svg")));
    lbProtocol->setScaledContents(false);
    lbRevert = new QLabel(frame);
    lbRevert->setObjectName(QString::fromUtf8("lbRevert"));
    lbRevert->setGeometry(QRect(870, 10, 51, 61));
    lbRevert->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/gtk-undo-ltr.svg")));
    lbRevert->setScaledContents(false);
    swSettings = new QStackedWidget(SettingsDialog);
    swSettings->setObjectName(QString::fromUtf8("swSettings"));
    swSettings->setGeometry(QRect(10, 130, 981, 361));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    gridLayout = new QWidget(page);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setGeometry(QRect(0, 0, 652, 351));
    gridLayout1 = new QGridLayout(gridLayout);
    gridLayout1->setSpacing(6);
    gridLayout1->setMargin(0);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 8, 2, 1, 1);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem1, 12, 2, 1, 1);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem2, 8, 1, 1, 1);

    lePrompts = new QLineEdit(gridLayout);
    lePrompts->setObjectName(QString::fromUtf8("lePrompts"));

    gridLayout1->addWidget(lePrompts, 11, 4, 1, 1);

    spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem3, 4, 1, 1, 1);

    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem4, 5, 3, 1, 1);

    spacerItem5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem5, 4, 0, 1, 1);

    spacerItem6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem6, 12, 4, 1, 1);

    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem7, 8, 3, 1, 1);

    spacerItem8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem8, 6, 4, 1, 1);

    spacerItem9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem9, 4, 3, 1, 1);

    leVocab = new QLineEdit(gridLayout);
    leVocab->setObjectName(QString::fromUtf8("leVocab"));

    gridLayout1->addWidget(leVocab, 10, 4, 1, 1);

    spacerItem10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem10, 5, 2, 1, 1);

    cbStartSimonOnBoot = new QCheckBox(gridLayout);
    cbStartSimonOnBoot->setObjectName(QString::fromUtf8("cbStartSimonOnBoot"));
    cbStartSimonOnBoot->setChecked(true);

    gridLayout1->addWidget(cbStartSimonOnBoot, 5, 1, 1, 1);

    spacerItem11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem11, 2, 2, 1, 1);

    spacerItem12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem12, 2, 3, 1, 1);

    spacerItem13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem13, 4, 4, 1, 1);

    spacerItem14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem14, 6, 2, 1, 1);

    spacerItem15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem15, 9, 2, 1, 1);

    lbJuliusdAddr = new QLabel(gridLayout);
    lbJuliusdAddr->setObjectName(QString::fromUtf8("lbJuliusdAddr"));

    gridLayout1->addWidget(lbJuliusdAddr, 3, 0, 1, 1);

    lbGrammar = new QLabel(gridLayout);
    lbGrammar->setObjectName(QString::fromUtf8("lbGrammar"));

    gridLayout1->addWidget(lbGrammar, 11, 0, 1, 1);

    spacerItem16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem16, 12, 3, 1, 1);

    lbFiles = new QLabel(gridLayout);
    lbFiles->setObjectName(QString::fromUtf8("lbFiles"));

    gridLayout1->addWidget(lbFiles, 8, 0, 1, 1);

    spacerItem17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem17, 2, 1, 1, 1);

    spacerItem18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem18, 6, 0, 1, 1);

    lbConfig = new QLabel(gridLayout);
    lbConfig->setObjectName(QString::fromUtf8("lbConfig"));

    gridLayout1->addWidget(lbConfig, 9, 0, 1, 1);

    spacerItem19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem19, 7, 4, 1, 1);

    spacerItem20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem20, 6, 3, 1, 1);

    spacerItem21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem21, 7, 3, 1, 1);

    spacerItem22 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem22, 12, 0, 1, 1);

    lbAutostart = new QLabel(gridLayout);
    lbAutostart->setObjectName(QString::fromUtf8("lbAutostart"));

    gridLayout1->addWidget(lbAutostart, 5, 0, 1, 1);

    spacerItem23 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem23, 12, 1, 1, 1);

    leGrammar = new QLineEdit(gridLayout);
    leGrammar->setObjectName(QString::fromUtf8("leGrammar"));

    gridLayout1->addWidget(leGrammar, 11, 1, 1, 1);

    spacerItem24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem24, 11, 2, 1, 1);

    leConfig = new QLineEdit(gridLayout);
    leConfig->setObjectName(QString::fromUtf8("leConfig"));

    gridLayout1->addWidget(leConfig, 9, 1, 1, 1);

    leCommands = new QLineEdit(gridLayout);
    leCommands->setObjectName(QString::fromUtf8("leCommands"));

    gridLayout1->addWidget(leCommands, 9, 4, 1, 1);

    spacerItem25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem25, 3, 2, 1, 1);

    spacerItem26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem26, 0, 3, 1, 1);

    spacerItem27 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem27, 7, 0, 1, 1);

    lbVocab = new QLabel(gridLayout);
    lbVocab->setObjectName(QString::fromUtf8("lbVocab"));

    gridLayout1->addWidget(lbVocab, 10, 3, 1, 1);

    spacerItem28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem28, 0, 2, 1, 1);

    spacerItem29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem29, 10, 2, 1, 1);

    spacerItem30 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem30, 0, 4, 1, 1);

    cbStartJuliusdOnBoot = new QCheckBox(gridLayout);
    cbStartJuliusdOnBoot->setObjectName(QString::fromUtf8("cbStartJuliusdOnBoot"));
    cbStartJuliusdOnBoot->setChecked(true);

    gridLayout1->addWidget(cbStartJuliusdOnBoot, 5, 4, 1, 1);

    lbLexicon = new QLabel(gridLayout);
    lbLexicon->setObjectName(QString::fromUtf8("lbLexicon"));

    gridLayout1->addWidget(lbLexicon, 10, 0, 1, 1);

    cbStartJuliusAsNeeded = new QCheckBox(gridLayout);
    cbStartJuliusAsNeeded->setObjectName(QString::fromUtf8("cbStartJuliusAsNeeded"));
    cbStartJuliusAsNeeded->setChecked(true);

    gridLayout1->addWidget(cbStartJuliusAsNeeded, 6, 1, 1, 1);

    spacerItem31 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem31, 8, 4, 1, 1);

    spacerItem32 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem32, 7, 1, 1, 1);

    sbPort = new QSpinBox(gridLayout);
    sbPort->setObjectName(QString::fromUtf8("sbPort"));
    sbPort->setMaximum(10000);
    sbPort->setMinimum(4000);
    sbPort->setValue(4444);

    gridLayout1->addWidget(sbPort, 3, 4, 1, 1);

    lbJuliusdPort = new QLabel(gridLayout);
    lbJuliusdPort->setObjectName(QString::fromUtf8("lbJuliusdPort"));

    gridLayout1->addWidget(lbJuliusdPort, 3, 3, 1, 1);

    label = new QLabel(gridLayout);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout1->addWidget(label, 0, 0, 1, 1);

    leAddress = new QLineEdit(gridLayout);
    leAddress->setObjectName(QString::fromUtf8("leAddress"));

    gridLayout1->addWidget(leAddress, 3, 1, 1, 1);

    lbPrompts = new QLabel(gridLayout);
    lbPrompts->setObjectName(QString::fromUtf8("lbPrompts"));

    gridLayout1->addWidget(lbPrompts, 11, 3, 1, 1);

    spacerItem33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem33, 2, 4, 1, 1);

    spacerItem34 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem34, 4, 2, 1, 1);

    leLexicon = new QLineEdit(gridLayout);
    leLexicon->setObjectName(QString::fromUtf8("leLexicon"));

    gridLayout1->addWidget(leLexicon, 10, 1, 1, 1);

    lbCommands1 = new QLabel(gridLayout);
    lbCommands1->setObjectName(QString::fromUtf8("lbCommands1"));

    gridLayout1->addWidget(lbCommands1, 9, 3, 1, 1);

    lbJuliusd = new QLabel(gridLayout);
    lbJuliusd->setObjectName(QString::fromUtf8("lbJuliusd"));

    gridLayout1->addWidget(lbJuliusd, 2, 0, 1, 1);

    spacerItem35 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem35, 7, 2, 1, 1);

    spacerItem36 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem36, 1, 0, 1, 1);

    spacerItem37 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem37, 1, 1, 1, 1);

    spacerItem38 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem38, 1, 2, 1, 1);

    spacerItem39 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem39, 1, 3, 1, 1);

    spacerItem40 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem40, 1, 4, 1, 1);

    checkBox = new QCheckBox(gridLayout);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));

    gridLayout1->addWidget(checkBox, 0, 1, 1, 1);

    liSep1 = new QFrame(page);
    liSep1->setObjectName(QString::fromUtf8("liSep1"));
    liSep1->setGeometry(QRect(660, 0, 20, 351));
    liSep1->setFrameShape(QFrame::VLine);
    liSep1->setFrameShadow(QFrame::Sunken);
    lbHelpSystem = new QLabel(page);
    lbHelpSystem->setObjectName(QString::fromUtf8("lbHelpSystem"));
    lbHelpSystem->setGeometry(QRect(690, 20, 271, 331));
    lbHelpSystem->setWordWrap(true);
    swSettings->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    swSettings->addWidget(page_2);
    pbConfirm = new QPushButton(SettingsDialog);
    pbConfirm->setObjectName(QString::fromUtf8("pbConfirm"));
    pbConfirm->setGeometry(QRect(850, 510, 141, 31));
    pbConfirm->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-save.svg")));
    pbApply = new QPushButton(SettingsDialog);
    pbApply->setObjectName(QString::fromUtf8("pbApply"));
    pbApply->setGeometry(QRect(700, 510, 141, 31));
    pbApply->setIcon(QIcon(QString::fromUtf8(":/images/icons/view-refresh.svg")));
    pbBack = new QPushButton(SettingsDialog);
    pbBack->setObjectName(QString::fromUtf8("pbBack"));
    pbBack->setGeometry(QRect(10, 510, 141, 31));
    pbBack->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-unreadable.svg")));
    QWidget::setTabOrder(pbSystemSettings, pbSoundSettings);
    QWidget::setTabOrder(pbSoundSettings, pbCommands);
    QWidget::setTabOrder(pbCommands, pbProtocol);
    QWidget::setTabOrder(pbProtocol, pbRevert);
    QWidget::setTabOrder(pbRevert, leAddress);
    QWidget::setTabOrder(leAddress, sbPort);
    QWidget::setTabOrder(sbPort, cbStartSimonOnBoot);
    QWidget::setTabOrder(cbStartSimonOnBoot, cbStartJuliusdOnBoot);
    QWidget::setTabOrder(cbStartJuliusdOnBoot, cbStartJuliusAsNeeded);
    QWidget::setTabOrder(cbStartJuliusAsNeeded, leConfig);
    QWidget::setTabOrder(leConfig, leCommands);
    QWidget::setTabOrder(leCommands, leLexicon);
    QWidget::setTabOrder(leLexicon, leVocab);
    QWidget::setTabOrder(leVocab, leGrammar);
    QWidget::setTabOrder(leGrammar, lePrompts);
    QWidget::setTabOrder(lePrompts, pbBack);
    QWidget::setTabOrder(pbBack, pbConfirm);

    retranslateUi(SettingsDialog);

    QSize size(996, 551);
    size = size.expandedTo(SettingsDialog->minimumSizeHint());
    SettingsDialog->resize(size);

    QObject::connect(pbBack, SIGNAL(clicked()), SettingsDialog, SLOT(reject()));
    QObject::connect(pbConfirm, SIGNAL(clicked()), SettingsDialog, SLOT(accept()));

    swSettings->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
    SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Einstellungen", 0, QApplication::UnicodeUTF8));
    pbRevert->setText(QApplication::translate("SettingsDialog", "Zur\303\274cksetzen", 0, QApplication::UnicodeUTF8));
    lbSystemSettings->setToolTip(QApplication::translate("SettingsDialog", "Die Haupt-Systemeinstellungen", 0, QApplication::UnicodeUTF8));
    lbSystemSettings->setText(QString());
    pbSystemSettings->setToolTip(QApplication::translate("SettingsDialog", "Die Haupt-Systemeinstellungen", 0, QApplication::UnicodeUTF8));
    pbSystemSettings->setText(QApplication::translate("SettingsDialog", "Systemeinstellungen", 0, QApplication::UnicodeUTF8));
    lbSoundSettings->setToolTip(QApplication::translate("SettingsDialog", "Alle einstellungen rund um die Aufnahmen und das verwendete Soundsystem", 0, QApplication::UnicodeUTF8));
    lbSoundSettings->setText(QString());
    pbSoundSettings->setToolTip(QApplication::translate("SettingsDialog", "Alle einstellungen rund um die Aufnahmen und das verwendete Soundsystem", 0, QApplication::UnicodeUTF8));
    pbSoundSettings->setText(QApplication::translate("SettingsDialog", "Soundeinstellungen", 0, QApplication::UnicodeUTF8));
    lbCommands->setToolTip(QApplication::translate("SettingsDialog", "Kommandos die simon veranlasst auf gewisse Schl\303\274sselw\303\266rter spezielle Aktionen auszuf\303\274hren", 0, QApplication::UnicodeUTF8));
    lbCommands->setText(QString());
    pbCommands->setToolTip(QApplication::translate("SettingsDialog", "Kommandos die simon veranlasst auf gewisse Schl\303\274sselw\303\266rter spezielle Aktionen auszuf\303\274hren", 0, QApplication::UnicodeUTF8));
    pbCommands->setText(QApplication::translate("SettingsDialog", "Kommandos", 0, QApplication::UnicodeUTF8));
    pbProtocol->setToolTip(QApplication::translate("SettingsDialog", "Einstellungen \303\274ber die Protokolle von simon", 0, QApplication::UnicodeUTF8));
    pbProtocol->setText(QApplication::translate("SettingsDialog", "Protokolle", 0, QApplication::UnicodeUTF8));
    lbProtocol->setToolTip(QApplication::translate("SettingsDialog", "Einstellungen \303\274ber die Protokolle von simon", 0, QApplication::UnicodeUTF8));
    lbProtocol->setText(QString());
    lbRevert->setToolTip(QApplication::translate("SettingsDialog", "Setzt das System auf einen fr\303\274heren Status zur\303\274ck", 0, QApplication::UnicodeUTF8));
    lbRevert->setText(QString());
    lePrompts->setText(QApplication::translate("SettingsDialog", "model/prompts", 0, QApplication::UnicodeUTF8));
    leVocab->setText(QApplication::translate("SettingsDialog", "model/model.voca", 0, QApplication::UnicodeUTF8));
    cbStartSimonOnBoot->setText(QApplication::translate("SettingsDialog", "Starte simon bei Systemstart", 0, QApplication::UnicodeUTF8));
    lbJuliusdAddr->setText(QApplication::translate("SettingsDialog", "Addresse zu Juliusd:", 0, QApplication::UnicodeUTF8));
    lbGrammar->setText(QApplication::translate("SettingsDialog", "Grammatik:", 0, QApplication::UnicodeUTF8));
    lbFiles->setText(QApplication::translate("SettingsDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Dateien:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    lbConfig->setText(QApplication::translate("SettingsDialog", "Konfiguration:", 0, QApplication::UnicodeUTF8));
    lbAutostart->setText(QApplication::translate("SettingsDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Autostart:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    leGrammar->setText(QApplication::translate("SettingsDialog", "model/model.grammar", 0, QApplication::UnicodeUTF8));
    leConfig->setText(QApplication::translate("SettingsDialog", "conf/settings.xml", 0, QApplication::UnicodeUTF8));
    leCommands->setText(QApplication::translate("SettingsDialog", "conf/commands.xml", 0, QApplication::UnicodeUTF8));
    lbVocab->setText(QApplication::translate("SettingsDialog", "Vokabular:", 0, QApplication::UnicodeUTF8));
    cbStartJuliusdOnBoot->setText(QApplication::translate("SettingsDialog", "Starte juliusd bei Systemstart", 0, QApplication::UnicodeUTF8));
    lbLexicon->setText(QApplication::translate("SettingsDialog", "Lexikon:", 0, QApplication::UnicodeUTF8));
    cbStartJuliusAsNeeded->setText(QApplication::translate("SettingsDialog", "Starte juliusd wenn n\303\266tig", 0, QApplication::UnicodeUTF8));
    lbJuliusdPort->setText(QApplication::translate("SettingsDialog", "Port:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("SettingsDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Allgemein:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    leAddress->setText(QApplication::translate("SettingsDialog", "127.0.0.1", 0, QApplication::UnicodeUTF8));
    lbPrompts->setText(QApplication::translate("SettingsDialog", "Prompts:", 0, QApplication::UnicodeUTF8));
    leLexicon->setText(QApplication::translate("SettingsDialog", "model/lexicon", 0, QApplication::UnicodeUTF8));
    lbCommands1->setText(QApplication::translate("SettingsDialog", "Kommandos:", 0, QApplication::UnicodeUTF8));
    lbJuliusd->setText(QApplication::translate("SettingsDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Juliusd:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("SettingsDialog", "Nachfragen vor dem Beenden", 0, QApplication::UnicodeUTF8));
    lbHelpSystem->setText(QApplication::translate("SettingsDialog", "simon verwendet f\303\274r die Erkennung selbst die Open Source Spracherkennungs-engine \"Julius\".\n"
"Die Verbindung zu Julius wird \303\274ber das Netzwerk aufgebaut.\n"
"Die Addresse zum Juliusd wird entweder mit entweder mit einem Hostnamen oder einer direkten \"IP Adresse\" angegeben (4x3 stellige Netzwerksidentifikationsnummer - xxx.xxx.xxx.xxx)\n"
"Die Port Nummer muss beim Juliusd und bei simon gleich sein.", 0, QApplication::UnicodeUTF8));
    pbConfirm->setToolTip(QApplication::translate("SettingsDialog", "Speichert und \303\234bernimmt die Einstellungen und schlie\303\237t den Dialog", 0, QApplication::UnicodeUTF8));
    pbConfirm->setText(QApplication::translate("SettingsDialog", "&Ok", 0, QApplication::UnicodeUTF8));
    pbApply->setToolTip(QApplication::translate("SettingsDialog", "\303\234bernimmt die neuen Einstellungen", 0, QApplication::UnicodeUTF8));
    pbApply->setText(QApplication::translate("SettingsDialog", "\303\234&bernehmen", 0, QApplication::UnicodeUTF8));
    pbBack->setToolTip(QApplication::translate("SettingsDialog", "Verwirft alle \303\204nderungen und schlie\303\237t den Dialog", 0, QApplication::UnicodeUTF8));
    pbBack->setText(QApplication::translate("SettingsDialog", "&Abbrechen", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SettingsDialog);
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

#endif // UI_SETTINGS_H
