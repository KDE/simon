/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_SCENARIOMANAGER_H_5056473DC97C4A0BABECF3F11ACF3296
#define SIMON_SCENARIOMANAGER_H_5056473DC97C4A0BABECF3F11ACF3296

/**
 *	@class ScenarioManager
 *	@author Peter Grasch
 */

#include <QObject>
#include <QStringList>
#include <QList>
#include <QHash>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonactions/greedyreceiver.h>

#include "command.h"
#include "commandlistelements.h"
#include "speechmodel.h"
#include "vocabulary.h"
#include "simonmodelmanagement_export.h"

class Word;
class Scenario;
class ScenarioDisplay;
class ShadowVocabulary;
class Command;
class Action;
class CommandManager;
class VoiceInterfaceCommand;
class ScenarioOfferUi;

class MODELMANAGEMENT_EXPORT ScenarioManager : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("Simons scenario interface", "org.simon-listens.ScenarioManager")

  signals:
    void scenarioSelectionChanged();
    void scenariosChanged();
    void shadowVocabularyChanged();
    void baseModelChanged();
    void deactivatedScenarioListChanged();

  private:
    bool m_inGroup;
    bool m_baseModelDirty;
    bool m_scenariosDirty;
    bool m_shadowVocabularyDirty;
    bool setupScenario(Scenario *s);

    ShadowVocabulary *shadowVocab;
    Scenario *currentScenario;

    QList<Scenario*> scenarios;
    QList<ScenarioDisplay*> scenarioDisplays;
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands;

  public:
    static ScenarioManager *getInstance();

    enum ScenarioOfferReply {
      Accepted=1,
      Rejected=0,
      Incompatible=-1
    };

    QList<Scenario*> getScenarios() { return scenarios; }
    void registerScenarioDisplay(ScenarioDisplay *display);
    void deRegisterScenarioDisplay(ScenarioDisplay *display);

    Scenario *getScenario(const QString& id);
    Scenario *getCurrentScenario() { return currentScenario; }

    bool init();
    ShadowVocabulary* getShadowVocabulary();

    bool storeScenario(const QString& id, const QByteArray& data);

    QStringList getCategories(SpeechModel::ModelElements elements);
    bool renameCategory(const QString& category, const QString& newName, SpeechModel::ModelElements affect);

    QList<Word*> findWords(const QString& name, SpeechModel::ModelElements elements, Vocabulary::MatchType);
    QList<Word*> findWordsByCategory(const QString& name, SpeechModel::ModelElements elements);

    QStringList getExampleSentences(const QString& name, const QString& category, int count, SpeechModel::ModelElements elements);
    bool setupScenarios(bool forceChange=false);
    void setupAllChildScenarios();

    QStringList getAllAvailableScenarioIds(const QString& dataPrefix);
    QStringList getAllAvailableScenarioIds();

    QStringList getAllDeactivatedScenarioIds();

    bool triggerCommand(const QString& type, const QString& trigger, bool silent);

    bool processResult(RecognitionResult recognitionResult);

    CommandList getCommandList();

    void startGroup();
    bool commitGroup(bool silent=false);

    void setPluginFont(const QFont& font);

    void setListBaseConfiguration(QHash<CommandListElements::Element, VoiceInterfaceCommand*> listInterfaceCommands);

    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListBaseConfiguration();

    void touchBaseModelAccessTime();

    void installScenarioOfferUi(const ScenarioOfferUi* offerUi);

  public slots:
    // If force is true, every registered display will switch to this scenario
    // if not, only displays that already display the scenario will be updated
    void updateDisplays(Scenario* scenario, bool force=false);
    void scenarioActivationChanged();

    void slotBaseModelChanged();
    QHash< QString, QString > transcribe(QStringList words);
    QString transcribe(QString word);

    ScenarioOfferReply installScenario(const QString& requester, const QString& path);

  private:
    ScenarioManager(QObject *parent=0);
    ~ScenarioManager();
    static ScenarioManager *instance;

    const ScenarioOfferUi* m_scenarioOfferUi;
};


#endif
