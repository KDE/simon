/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMONPROTOCOL_H
#define SIMONPROTOCOL_H

namespace Simond
{
	enum Request {
		Ack=1,
		
		Login=1001,
		VersionIncompatible=1002,
		AuthenticationFailed=1003,
		AccessDenied=1004,
		LoginSuccessful=1005,
		
		
		StartSynchronisation=2001,
		
		GetActiveModelDate=2011,
		ActiveModelDate=2012,
		GetActiveModel=2013,
		ErrorRetrievingActiveModel=2014,
		ActiveModel=2015,
		NoActiveModelAvailable=2016,
		ActiveModelStorageFailed=2017,
		
		
		GetModelSrcDate=2021,
		ModelSrcDate=2022,
		
		GetWordListDate=2031,
		WordListDate=2032,
		GetWordList=2033,
		ErrorRetrievingWordList=2034,
		WordList=2035,
		NoWordListAvailable=2036,
		WordListStorageFailed=2037,
		
		GetGrammarDate=2041,
		GrammarDate=2042,
		GetGrammar=2043,
		ErrorRetrievingGrammar=2044,
		Grammar=2045,
		NoGrammarAvailable=2046,
		GrammarStorageFailed=2047,
		
		GetLanguageDescriptionDate=2051,
		LanguageDescriptionDate=2052,
		GetLanguageDescription=2053,
		ErrorRetrievingLanguageDescription=2054,
		LanguageDescription=2055,
		NoLanguageDescriptionAvailable=2056,
		LanguageDescriptionStorageFailed=2057,
		
		GetTrainingDate=2061,
		TrainingDate=2062,
		GetTraining=2063,
		ErrorRetrievingTraining=2064,
		Training=2065,
		NoTrainingAvailable=2066,
		TrainingStorageFailed=2067,
		
		
		GetTrainingsSample=2071,
		ErrorRetrievingTrainingsSample=2072,
		TrainingsSample=2073,
		TrainingsSampleStorageFailed=2074,
		
		SynchronisationComplete=2101,
		
		ModelCompilationStarted=3001,
		ModelCompilationStatus=3002,
		ModelCompilationError=3003,
		
		RecognitionReady=4001,
		StartRecognition=4002,
		RecognitionError=4003,
		RecognitionWarning=4004,
		RecognitionStarted=4005,
		StopRecognition=4006,
		RecognitionStopped=4007,
		RecognitionTemporarilyUnavailable=4008,
		RecognitionResult=4009
	};
};

#endif
