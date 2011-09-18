#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include "recognitionresult.h"

class Contacts;
class Destinations;

class ActionManager : public QObject
{
    Q_OBJECT
signals:
    void enterCallMode();
    void enterNavigationMode();
    void returnToMain();

private:
    enum Mode {
        Main,
        Call,
        Navigation
    };

    Contacts *contacts;
    Destinations *destinations;
    Mode currentMode;

    void parseCallActions(const QString& result);
    void parseNavigationActions(const QString& result);

public:
    explicit ActionManager(Contacts *_contacts, Destinations *_destinations, QObject *parent = 0);

signals:
    void actionExecuted(const QString& name);

public slots:
    void recognized(const RecognitionResultList& results);
    void mainMode();
    void callMode();
    void navigationMode();

};

#endif // ACTIONMANAGER_H
