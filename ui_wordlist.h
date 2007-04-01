/********************************************************************************
** Form generated from reading ui file 'wordlist.ui'
**
** Created: Sun Apr 1 21:29:14 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WORDLIST_H
#define UI_WORDLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

class Ui_WordList
{
public:
    QFrame *line_2;
    QLabel *label_2;
    QLabel *lbSearch_2;
    QLabel *lbSearch;
    QPushButton *pbAddToTraining;
    QPushButton *pbSuggestTrain_2;
    QPushButton *pbAddWord;
    QPushButton *pbAddWord_2;
    QLineEdit *leSearch;
    QPushButton *pbClearSearch;
    QLabel *lbHelp;
    QPushButton *pbBack;
    QFrame *line_3;
    QFrame *line;
    QPushButton *pbSwitchToTraining;
    QPushButton *pbDeleteTrainingWord;
    QPushButton *pbSuggestTrain;

    void setupUi(QDialog *WordList)
    {
    WordList->setObjectName(QString::fromUtf8("WordList"));
    WordList->setWindowIcon(QIcon(QString::fromUtf8(":/images/tray.png")));
    line_2 = new QFrame(WordList);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setGeometry(QRect(480, 380, 201, 16));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    label_2 = new QLabel(WordList);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(480, 12, 181, 17));
    lbSearch_2 = new QLabel(WordList);
    lbSearch_2->setObjectName(QString::fromUtf8("lbSearch_2"));
    lbSearch_2->setGeometry(QRect(17, 12, 134, 17));
    lbSearch = new QLabel(WordList);
    lbSearch->setObjectName(QString::fromUtf8("lbSearch"));
    lbSearch->setGeometry(QRect(250, 13, 61, 17));
    pbAddToTraining = new QPushButton(WordList);
    pbAddToTraining->setObjectName(QString::fromUtf8("pbAddToTraining"));
    pbAddToTraining->setGeometry(QRect(470, 220, 221, 31));
    pbAddToTraining->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-up.svg")));
    pbSuggestTrain_2 = new QPushButton(WordList);
    pbSuggestTrain_2->setObjectName(QString::fromUtf8("pbSuggestTrain_2"));
    pbSuggestTrain_2->setGeometry(QRect(470, 340, 221, 31));
    QFont font;
    font.setItalic(true);
    pbSuggestTrain_2->setFont(font);
    pbSuggestTrain_2->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-next.svg")));
    pbAddWord = new QPushButton(WordList);
    pbAddWord->setObjectName(QString::fromUtf8("pbAddWord"));
    pbAddWord->setGeometry(QRect(470, 400, 221, 31));
    pbAddWord->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-new.svg")));
    pbAddWord_2 = new QPushButton(WordList);
    pbAddWord_2->setObjectName(QString::fromUtf8("pbAddWord_2"));
    pbAddWord_2->setGeometry(QRect(470, 440, 221, 31));
    pbAddWord_2->setIcon(QIcon(QString::fromUtf8(":/images/icons/process-stop.svg")));
    leSearch = new QLineEdit(WordList);
    leSearch->setObjectName(QString::fromUtf8("leSearch"));
    leSearch->setGeometry(QRect(310, 10, 113, 25));
    pbClearSearch = new QPushButton(WordList);
    pbClearSearch->setObjectName(QString::fromUtf8("pbClearSearch"));
    pbClearSearch->setGeometry(QRect(430, 10, 16, 27));
    lbHelp = new QLabel(WordList);
    lbHelp->setObjectName(QString::fromUtf8("lbHelp"));
    lbHelp->setGeometry(QRect(720, 20, 270, 461));
    pbBack = new QPushButton(WordList);
    pbBack->setObjectName(QString::fromUtf8("pbBack"));
    pbBack->setGeometry(QRect(470, 510, 221, 31));
    pbBack->setIcon(QIcon(QString::fromUtf8(":/images/icons/go-previous.svg")));
    line_3 = new QFrame(WordList);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setGeometry(QRect(470, 490, 511, 16));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    line = new QFrame(WordList);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(700, 0, 20, 541));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    pbSwitchToTraining = new QPushButton(WordList);
    pbSwitchToTraining->setObjectName(QString::fromUtf8("pbSwitchToTraining"));
    pbSwitchToTraining->setGeometry(QRect(720, 510, 251, 31));
    pbSwitchToTraining->setIcon(QIcon(QString::fromUtf8(":/images/icons/document-properties.svg")));
    pbDeleteTrainingWord = new QPushButton(WordList);
    pbDeleteTrainingWord->setObjectName(QString::fromUtf8("pbDeleteTrainingWord"));
    pbDeleteTrainingWord->setGeometry(QRect(470, 260, 221, 31));
    pbDeleteTrainingWord->setIcon(QIcon(QString::fromUtf8(":/images/icons/emblem-unreadable.svg")));
    pbSuggestTrain = new QPushButton(WordList);
    pbSuggestTrain->setObjectName(QString::fromUtf8("pbSuggestTrain"));
    pbSuggestTrain->setGeometry(QRect(470, 300, 221, 31));
    pbSuggestTrain->setIcon(QIcon(QString::fromUtf8(":/images/icons/bookmark-new.svg")));
    lbSearch_2->setBuddy(leSearch);
    lbSearch->setBuddy(leSearch);
    QWidget::setTabOrder(leSearch, pbClearSearch);
    QWidget::setTabOrder(pbClearSearch, pbAddToTraining);
    QWidget::setTabOrder(pbAddToTraining, pbDeleteTrainingWord);
    QWidget::setTabOrder(pbDeleteTrainingWord, pbSuggestTrain);
    QWidget::setTabOrder(pbSuggestTrain, pbAddWord);
    QWidget::setTabOrder(pbAddWord, pbAddWord_2);
    QWidget::setTabOrder(pbAddWord_2, pbBack);

    retranslateUi(WordList);

    QSize size(997, 552);
    size = size.expandedTo(WordList->minimumSizeHint());
    WordList->resize(size);


    QMetaObject::connectSlotsByName(WordList);
    } // setupUi

    void retranslateUi(QDialog *WordList)
    {
    WordList->setWindowTitle(QApplication::translate("WordList", "Bekannte W\303\266rter", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("WordList", "Zum Trainieren ausgew\303\244hlt:", 0, QApplication::UnicodeUTF8));
    lbSearch_2->setText(QApplication::translate("WordList", "Gesamtes Vokabular:", 0, QApplication::UnicodeUTF8));
    lbSearch->setText(QApplication::translate("WordList", "&Suchen:", 0, QApplication::UnicodeUTF8));
    pbAddToTraining->setToolTip(QApplication::translate("WordList", "F\303\274gt das in der Liste links selektierte Wort in die Liste der zu trainierenden W\303\266rter (oben) ein", 0, QApplication::UnicodeUTF8));
    pbAddToTraining->setText(QApplication::translate("WordList", "Zum Training &hinzuf\303\274gen", 0, QApplication::UnicodeUTF8));
    pbSuggestTrain_2->setToolTip(QApplication::translate("WordList", "Trainiert die Liste der W\303\266rter", 0, QApplication::UnicodeUTF8));
    pbSuggestTrain_2->setText(QApplication::translate("WordList", "&Trainieren", 0, QApplication::UnicodeUTF8));
    pbAddWord->setToolTip(QApplication::translate("WordList", "F\303\274gt ein neues Wort in das Sprachmodell ein", 0, QApplication::UnicodeUTF8));
    pbAddWord->setText(QApplication::translate("WordList", "&Neues Wort hinzuf\303\274gen", 0, QApplication::UnicodeUTF8));
    pbAddWord_2->setToolTip(QApplication::translate("WordList", "L\303\266scht das Wort komplett aus dem Sprachmodell", 0, QApplication::UnicodeUTF8));
    pbAddWord_2->setText(QApplication::translate("WordList", "Wort &l\303\266schen", 0, QApplication::UnicodeUTF8));
    leSearch->setToolTip(QApplication::translate("WordList", "Schr\303\244nkt die Liste ein", 0, QApplication::UnicodeUTF8));
    pbClearSearch->setToolTip(QApplication::translate("WordList", "Setzt die Sucheingabe zur\303\274ck", 0, QApplication::UnicodeUTF8));
    pbClearSearch->setText(QApplication::translate("WordList", "&x", 0, QApplication::UnicodeUTF8));
    lbHelp->setText(QApplication::translate("WordList", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hilfe:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Das ist die Hauptansicht f\303\274r alle Aktionen</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">rund um das Spachmodell.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hier kannst du </p>\n"
""
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"> * Einzelne W\303\266rter trainieren</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">    <span style=\" font-style:italic;\">F\303\274r das allgemeine Training gibt es im </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-style:italic;\">    Hauptmen\303\274 einen extra Punkt </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-style:italic;\">    <span style=\" font-weight:600;\">\"Trainieren\"</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"> * W\303\266rter hinzuf\303\274gen</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right"
        ":0px; -qt-block-indent:0; text-indent:0px;\"> * W\303\266rter l\303\266schen</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Um einzelne W\303\266rter zu trainieren, markiere </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">das Wort in der Tabelle links und ziehe es</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">in das Feld oben in der Mitte.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Alternativ kannst du es auch im linken</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; tex"
        "t-indent:0px;\">Auswahlfeld selektieren und den Knopf</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">\"Zum Training hinzuf\303\274gen\" </span>dr\303\274cken.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Der Knopf <span style=\" font-weight:600;\">\"Trainingsprogramm </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\">vorschlagen\" <span style=\" font-weight:400;\">selektiert automatisch </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">zehn W\303\266rter die noch</p>\n"
"<p s"
        "tyle=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Verbesserungspotential besitzen</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Der Knopf \"<span style=\" font-weight:600;\">Neues Wort hinzuf\303\274gen</span>\"</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\303\266ffnet einen neuen Dialog der Ihnen</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">helfen wird ein neues Wort hinzuzuf\303\274gen.</p></body></html>", 0, QApplication::UnicodeUTF8));
    pbBack->setToolTip(QApplication::translate("WordList", "Schlie\303\237t den Dialog", 0, QApplication::UnicodeUTF8));
    pbBack->setText(QApplication::translate("WordList", "  &Zur\303\274ck", 0, QApplication::UnicodeUTF8));
    pbSwitchToTraining->setToolTip(QApplication::translate("WordList", "Schlie\303\237t den Dialog und wechselt zum generischen Training (Vorlesen vorgefertigter Texte)", 0, QApplication::UnicodeUTF8));
    pbSwitchToTraining->setText(QApplication::translate("WordList", "Zum generischen Training wechseln", 0, QApplication::UnicodeUTF8));
    pbDeleteTrainingWord->setToolTip(QApplication::translate("WordList", "L\303\266scht das Wort aus der Liste der zu trainierenden W\303\266rter", 0, QApplication::UnicodeUTF8));
    pbDeleteTrainingWord->setText(QApplication::translate("WordList", "Wort aus Liste &l\303\266schen", 0, QApplication::UnicodeUTF8));
    pbSuggestTrain->setToolTip(QApplication::translate("WordList", "Schl\303\244gt automatisch ein Training vor das die schw\303\244chsten 10 W\303\266rter trainiert.", 0, QApplication::UnicodeUTF8));
    pbSuggestTrain->setText(QApplication::translate("WordList", "Trainingsprog. &vorschlagen", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(WordList);
    } // retranslateUi

};

namespace Ui {
    class WordList: public Ui_WordList {};
} // namespace Ui

#endif // UI_WORDLIST_H
