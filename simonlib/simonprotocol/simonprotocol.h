/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SIMONPROTOCOL_H_F0E4B83DBA554FB0A9D83585242F6E2B
#define SIMON_SIMONPROTOCOL_H_F0E4B83DBA554FB0A9D83585242F6E2B

/*
 * "length" always refers to a 64bit signed integer (qint64) which contains the length of the rest of the packet
 * (excluding the 12 bytes of "header" data (qint32 type and the qint64 length itself))
 *
 * length is ONLY ommitted if it would be 0 (i.e. no data after the type) or if the length of the packet is not
 * variable
 */
namespace Simond
{
  enum Request
  {
    Login=1001,                                   /* qint64 length, quint8 protocol version, QByteArray username, QByteArray passwordHash */
    VersionIncompatible=1002,
    AuthenticationFailed=1003,
    AccessDenied=1004,
    LoginSuccessful=1005,

    StartSynchronisation=2001,
    SynchronisationInformation=2002,
    SynchronisationAlreadyRunning=2003,
    AbortSynchronisation=2004,
    AbortSynchronisationFailed=2005,

    GetBaseModel=2013,
    ErrorRetrievingBaseModel=2014,
    BaseModel=2015,
    BaseModelStorageFailed=2016,

    GetActiveModel=2023,
    ErrorRetrievingActiveModel=2024,
    GetActiveModelSampleRate=2025,
    ActiveModelSampleRate=2026,                   /* QDateTime modifiedDat, qint32 samplerate, QByteArray hmmDefs, QByteArray tiedlist, QByteArray dict, QByteArray dfa */
    ActiveModel=2027,                             /* QDateTime modifiedDat, qint32 samplerate, QByteArray hmmDefs, QByteArray tiedlist, QByteArray dict, QByteArray dfa */
    ActiveModelStorageFailed=2029,

    GetLanguageDescription=2053,
    ErrorRetrievingLanguageDescription=2054,
    LanguageDescription=2055,
    LanguageDescriptionStorageFailed=2057,

    GetTraining=2063,
    ErrorRetrievingTraining=2064,
    Training=2065,
    TrainingStorageFailed=2067,

    GetTrainingsSample=2072,
    ErrorRetrievingTrainingsSample=2073,
    TrainingsSample=2074,
    TrainingsSampleStorageFailed=2075,

    GetScenariosToDelete=2082,
    ScenariosToDelete=2083,

    GetScenarioList=2091,
    ErrorRetrievingScenarioList=2092,
    ScenarioList=2093,

    GetSelectedScenarioList=2096,
    ErrorRetrievingSelectedScenarioList=2097,
    SelectedScenarioList=2098,
    SelectedScenarioListStorageFailed=2099,

    GetDeactivatedScenarioList=2100,
    DeactivatedScenarioList=2101,

    GetDeactivatedSampleGroup=2110,
    DeactivatedSampleGroup=2111,

    GetScenario=2194,
    ErrorRetrievingScenario=2195,
    Scenario=2196,
    ScenarioStorageFailed=2197,
    ScenarioStored=2198,

    SynchronisationEndPending=2300,
    SynchronisationComplete=2301,
    SynchronisationCommitFailed=2302,

    GetAvailableModels=2401,
    ModelBackupDisabled=2402,
    AvailableModels=2403,
    SwitchToModel=2404,
    SwitchToModelFailed=2405,

    ModelCompilationStarted=3001,
    ModelCompilationStatus=3002,
    ModelCompilationError=3003,
    AbortModelCompilation=3004,
    ModelCompilationAborted=3005,
    ModelCompilationWordUndefined=3006,
    ModelCompilationPhonemeUndefined=3007,
    ModelCompilationClassUndefined=3008,
    GetModelCompilationProtocol=3009,
    ErrorRetrievingModelCompilationProtocol=3010,
    ModelCompilationProtocol=3011,
    ModelCompilationCompleted=3012,

    RecognitionReady=4001,
    StartRecognition=4002,
    RecognitionError=4003,
    RecognitionWarning=4004,
    RecognitionStarted=4006,
    StopRecognition=4007,
    RecognitionStopped=4008,
    RecognitionResult=4013,

    RecognitionStartSample=4021,
    RecognitionSampleData=4022,
    RecognitionSampleFinished=4023
  };
}
#endif
