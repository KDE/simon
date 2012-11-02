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

#include "sscqueries.h"
#include <QDebug>

/*
 * Returns the information on the user; This includes a human readable language name
 * from the language table. The returned view has the following fields:
 *
 * User::UserID
 * User::Surname
 * User::Forename
 * User::Sex
 * User::BirthYear
 * User::ZIPCode
 * User::Education
 * User::Occupation
 * User::MotherTongue (Alias: MotherTongueID)
 * Language::Name (Alias: MotherTongUser::)
 * User::Diagnosis
 * User::Orientation
 * User::MotorFunction
 * User::Communciation
 * User::MouthMotoric
 * User::InterviewPossible
 * User::RepeatPossible
 *
 * The query contains 1 placeholder:
 * 	:userid (maps to User::UserID)
 */
QSqlQuery SSCQueries::getUser()
{
  QSqlQuery q;
  q.prepare("SELECT u.UserId, u.Surname, u.Forename, u.Sex, u.BirthYear, "
    "u.ZIPCode, u.Education, u.Occupation, "
    "u.MotherTongue as MotherTongueID, l.Name as MotherTongue, "
    "u.Diagnosis, u.Orientation, u.MotorFunction, u.Communication, u.MouthMotoric, "
    "u.InterviewPossible, u.RepeatPossible from User u INNER JOIN "
    "Language l ON u.MotherTongue=l.LanguageId WHERE u.UserId = :userid");
  return q;
}


/*
 * Returns the information on the users matching the given criteria; This
 * includes a human readable language name from the language table.
 *
 * The placeholders can be controlled with the appropriate flags. To
 * limit the users by their sex for example, set includeSex = true and
 * you will get the placeholder :sex which you can bind with the desired value
 *
 * The return set will be limited to 50 entries!
 *
 * The returned view has the following fields:
 *
 * User::UserID
 * User::Surname
 * User::Forename
 * User::Sex
 * User::BirthYear
 * User::ZIPCode
 * User::Education
 * User::Occupation
 * User::MotherTongue (Alias: MotherTongueID)
 * Language::Name (Alias: MotherTongUser::)
 * User::Diagnosis
 * User::Orientation
 * User::MotorFunction
 * User::Communciation
 * User::MouthMotoric
 * User::InterviewPossible
 * User::RepeatPossible
 *
 * The query contains (up to) 18 placeholders:
 * 	:userid (maps to User::UserID)
 */

QSqlQuery SSCQueries::getUsers(bool includeUserId, bool includeSurname,
bool includeGivenName, bool includeSex, bool includeBirthYear,
bool includeZipcode, bool includeEducation, bool includeCurrentOccupation,
bool includeMotherTongue, bool includeDiagnosis, bool includeOrientation,
bool includeMotorfunction, bool includeCommunication, bool includeMouthMotoric,
bool includeInterviewPossible, bool includeRepeatingPossible, bool includeInstitutionId,
bool includeReferenceId)
{
  QString query = "SELECT u.UserId, u.Surname, u.Forename, u.Sex, u.BirthYear, "
    "u.ZIPCode, u.Education, u.Occupation, "
    "u.MotherTongue as MotherTongueID, l.Name as MotherTongue, "
    "u.Diagnosis, u.Orientation, u.MotorFunction, u.Communication, u.MouthMotoric, "
    "u.InterviewPossible, u.RepeatPossible from User u INNER JOIN "
    "Language l ON u.MotherTongue=l.LanguageId LEFT OUTER JOIN UserInInstitution "
    "uii ON u.UserId = uii.UserId WHERE 1=1 ";

  if (includeUserId)
    query += " AND u.UserId = :userid";
  if (includeSurname)
    query += " AND u.Surname LIKE :surname";
  if (includeGivenName)
    query += " AND u.Forename LIKE :forename";
  if (includeSex)
    query += " AND u.Sex = :sex";
  if (includeBirthYear)
    query += " AND u.BirthYear = :birthyear";
  if (includeZipcode)
    query += " AND u.ZIPCode LIKE :zipcode";
  if (includeEducation)
    query += " AND u.Education LIKE :education";
  if (includeCurrentOccupation)
    query += " AND u.Occupation LIKE :occupation";
  if (includeMotherTongue)
    query += " AND u.MotherTongue = :mothertongue";
  if (includeDiagnosis)
    query += " AND u.Diagnosis LIKE :diagnosis";
  if (includeOrientation)
    query += " AND u.Orientation = :orientation";
  if (includeMotorfunction)
    query += " AND u.MotorFunction = :motorfunction";
  if (includeCommunication)
    query += " AND u.Communication = :communication";
  if (includeMouthMotoric)
    query += " AND u.MouthMotoric LIKE :mouthmotoric";
  if (includeInterviewPossible)
    query += " AND u.InterviewPossible = :interviewpossible";
  if (includeRepeatingPossible)
    query += " AND u.RepeatPossible = :repeatpossible";
  if (includeInstitutionId)
    query += " AND uii.InstitutionId = :institutionid";
  if (includeReferenceId)
    query += " AND uii.InstitutionReferenceId = :referenceid";

  query += " GROUP BY u.UserId, u.Surname, u.Forename, u.Sex, u.BirthYear, "
    "u.ZIPCode, u.Education, u.Occupation, "
    "u.MotherTongue, l.Name, "
    "u.Diagnosis, u.Orientation, u.MotorFunction, u.Communication, u.MouthMotoric, "
    "u.InterviewPossible, u.RepeatPossible LIMIT 50";

  QSqlQuery q;
  q.prepare(query);

  return q;

}


/*
 * Adds a new user to the database
 *
 * The query contains 15 placeholders:
 * 	:surname (maps to User::Surname)
 * 	:forename (maps to User::Forename)
 * 	:sex (maps to User::Sex)
 * 	:birthyear (maps to User::BirthYear)
 * 	:zipcode (maps to User::ZIPCode)
 * 	:education (maps to User::Education)
 * 	:currentoccupation (maps to User::Occupation)
 * 	:mothertongue (maps to User::MotherTongue)
 * 	:diagnosis (maps to User::Diagnosis)
 * 	:orientation (maps to User::Orientation)
 * 	:motorfunction (maps to User::MotorFunction)
 * 	:communication (maps to User::Communciation)
 * 	:mouthmotoric (maps to User::MouthMotoric)
 * 	:interviewpossible (maps to User::InterviewPossible)
 * 	:repeatpossible (maps to User::RepeatPossible)
 */
QSqlQuery SSCQueries::addUser()
{
  QSqlQuery q;
  q.prepare("INSERT INTO User (Surname, Forename, Sex, BirthYear, "
    "ZIPCode, Education, Occupation, MotherTongue, "
    "Diagnosis, Orientation, MotorFunction, Communication, MouthMotoric, "
    "InterviewPossible, RepeatPossible) VALUES (:surname, :forename, "
    ":sex, :birthyear, :zipcode, :education, :currentoccupation, "
    ":mothertongue, :diagnosis, :orientation, :motorfunction, "
    ":communication, :mouthmotoric, "
    ":interviewpossible, :repeatpossible);");
  return q;
}


/*
 * Modifies a user
 *
 * The query contains 16 placeholders:
 * 	:userid (maps to User::UserID)
 * 	:surname (maps to User::Surname)
 * 	:forename (maps to User::Forename)
 * 	:sex (maps to User::Sex)
 * 	:birthyear (maps to User::BirthYear)
 * 	:zipcode (maps to User::ZIPCode)
 * 	:education (maps to User::Education)
 * 	:currentoccupation (maps to User::Occupation)
 * 	:mothertongue (maps to User::MotherTongue)
 * 	:diagnosis (maps to User::Diagnosis)
 * 	:orientation (maps to User::Orientation)
 * 	:motorfunction (maps to User::MotorFunction)
 * 	:communication (maps to User::Communciation)
 * 	:mouthmotoric (maps to User::MouthMotoric)
 * 	:interviewpossible (maps to User::InterviewPossible)
 * 	:repeatpossible (maps to User::RepeatPossible)
 */
QSqlQuery SSCQueries::modifyUser()
{
  QSqlQuery q;
  q.prepare("UPDATE User SET Surname=:surname, Forename=:forename, "
    "Sex=:sex, BirthYear=:birthyear, ZIPCode=:zipcode, "
    "Education=:education, Occupation=:currentoccupation, "
    "MotherTongue=:mothertongue, Diagnosis=:diagnosis, "
    "Orientation=:orientation, MotorFunction=:motorfunction, "
    "Communication=:communication, MouthMotoric=:mouthmotoric, "
    "InterviewPossible=:interviewpossible, "
    "RepeatPossible=:repeatpossible WHERE UserId=:userid;");
  return q;
}


/*
 * Removes the user from the database
 *
 * The query contains 1 placeholder:
 * 	:userid (maps to User::UserID)
 */
QSqlQuery SSCQueries::removeUser()
{
  QSqlQuery q;
  q.prepare("DELETE FROM User WHERE UserId = :userid");
  return q;
}


/*
 * Returns all available languages
 *
 * Language::LanguageId
 * Language::Name
 */
QSqlQuery SSCQueries::getLanguages()
{
  QSqlQuery q("SELECT LanguageId, Name from Language");
  return q;
}


/*
 * Returns all available microphones
 *
 * Microphone::MicrophoneId
 * Microphone::Model
 * Microphone::Type
 */
QSqlQuery SSCQueries::getMicrophones()
{
  QSqlQuery q("SELECT MicrophoneId, Model, Type from Microphone");
  return q;
}


/*
 * Returns all available soundcards
 *
 * SoundCard::SoundCardId
 * SoundCard::Model
 * SoundCard::Type
 */
QSqlQuery SSCQueries::getSoundCards()
{
  QSqlQuery q;
  q.prepare("SELECT SoundCardId, Model, Type from SoundCard");
  return q;
}


/*
 * Finds a microphone in the database
 *
 * The query contains 2 placeholders:
 * 	:model (maps to SoundCard::Model)
 * 	:type (maps to SoundCard::Type)
 */
QSqlQuery SSCQueries::getSoundCard()
{
  QSqlQuery q;
  q.prepare("SELECT SoundCardId, Model, Type from SoundCard WHERE Model = :model and Type = :type;");
  return q;
}


/*
 * Finds a sound card in the database
 *
 * The query contains 2 placeholders:
 * 	:model (maps to Microphone::Model)
 * 	:type (maps to Microphone::Type)
 */
QSqlQuery SSCQueries::getMicrophone()
{
  QSqlQuery q;
  q.prepare("SELECT MicrophoneId, Model, Type from Microphone WHERE Model = :model and Type = :type;");
  return q;
}


/*
 * Creates a new microphone
 *
 * The query contains 2 placeholders:
 * 	:model (maps to Microphone::Model)
 * 	:type (maps to Microphone::Type)
 */
QSqlQuery SSCQueries::createMicrophone()
{
  QSqlQuery q;
  q.prepare("INSERT INTO Microphone (Model, Type) VALUES (:model, :type)");
  return q;
}


/*
 * Creates a new sound card in the database
 *
 * The query contains 2 placeholders:
 * 	:model (maps to SoundCard::Model)
 * 	:type (maps to SoundCard::Type)
 */
QSqlQuery SSCQueries::createSoundCard()
{
  QSqlQuery q;
  q.prepare("INSERT INTO SoundCard (Model, Type) VALUES (:model, :type)");
  return q;
}


/*
 * Returns all available institutions
 *
 * Institution::InstitutionId
 * Institution::Name
 */
QSqlQuery SSCQueries::getInstitutions()
{
  QSqlQuery q;
  q.prepare("SELECT InstitutionId, Name from Institution");
  return q;
}


/*
 * Returns the selected institution
 *
 * Institution::InstitutionId
 * Institution::Name
 *
 * The query contains 1 placeholder:
 * 	:institutionid (maps to Institution::InstitutionId)
 */
QSqlQuery SSCQueries::getInstitution()
{
  QSqlQuery q;
  q.prepare("SELECT InstitutionId, Name from Institution WHERE Institution = :institutionid");
  return q;
}


/*
 * Adds a new institution to the database
 *
 * The query contains 1 placeholder:
 * 	:name (maps to Institution::Name)
 */
QSqlQuery SSCQueries::addInstitution()
{
  QSqlQuery q;
  q.prepare("INSERT INTO Institution (Name) VALUES "
    "(:name);");
  return q;

}


/*
 * Modifies a institution
 *
 * The query contains 2 placeholders:
 * 	:institutionid (maps to Institution::InstitutionID)
 * 	:name (maps to Institution::Name)
 */
QSqlQuery SSCQueries::modifyInstitution()
{
  QSqlQuery q;
  q.prepare("UPDATE Institution SET Name=:name WHERE InstitutionId=:institutionid;");
  return q;
}


/*
 * Removes the institution from the database
 *
 * The query contains 1 placeholder:
 * 	:institutionid (maps to Institution::InstitutionID)
 */
QSqlQuery SSCQueries::removeInstitution()
{
  QSqlQuery q;
  q.prepare("DELETE FROM Institution WHERE InstitutionId = :institutionid");
  return q;
}


/*
 * Returns all user - institution associations of the given user
 *
 * UserInInstitution::UserId
 * UserInInstitution::InstitutionId
 * UserInInstitution::InstitutionReferenceId
 *
 * The query contains 1 placeholder:
 * 	:userid (maps to UserInInstitution::UserId)
 */
QSqlQuery SSCQueries::getUserInstitutionAssociation()
{
  QSqlQuery q;
  q.prepare("SELECT UserId, InstitutionId, InstitutionReferenceId from UserInInstitution WHERE UserId = :userid;");
  return q;
}


/*
 * Adds a new user - institution association to the database
 *
 * The query contains 3 placeholders:
 * 	:userid (maps to UserInInstitution::UserId)
 * 	:institutionid (maps to UserInInstitution::InstitutionId)
 * 	:referenceid (maps to UserInInstitution::InstitutionReferenceId)
 */
QSqlQuery SSCQueries::addUserInstitutionAssociation()
{
  QSqlQuery q;
  q.prepare("INSERT INTO UserInInstitution (UserId, InstitutionId, InstitutionReferenceId) "
    "VALUES (:userid, :institutionid, "
    ":referenceid);");
  return q;
}


/*
 * Removes the user - institution association from the database
 *
 * The query contains 1 placeholder:
 * 	:userid (maps to UserInInstitution::UserId)
 * 	:institutionid (maps to UserInInstitution::InstitutionId)
 */
QSqlQuery SSCQueries::deleteUserInstitutionAssociation()
{
  QSqlQuery q;
  q.prepare("DELETE FROM UserInInstitution WHERE UserId = :userid AND InstitutionId = :institutionid");
  return q;
}


/*
 * Adds a new new sample to the database
 *
 * The query contains 4 placeholders:
 * 	:sampleid (maps to Sample::SampleId)
 * 	:userid (maps to Sample::UserId)
 * 	:typeid (maps to Sample::TypeId)
 * 	:microphoneid (maps to Sample::MicrophoneId)
 * 	:soundcardid (maps to Sample::SoundCardId)
 * 	:prompt (maps to Sample::Prompt)
 * 	:path (maps to Sample::Path)
 */
QSqlQuery SSCQueries::addSample()
{
  QSqlQuery q;
  q.prepare("INSERT INTO Sample (SampleId, UserId, TypeId, MicrophoneId, SoundCardId, Prompt, Path) "
    "VALUES (:sampleid, :userid, :typeid, :microphoneid, :soundcardid, "
    ":prompt, :path);");
  return q;
}


/*
 * Retrieves the highest sample id currently available
 *
 * Sample::SampleId
 */
QSqlQuery SSCQueries::lastSampleId()
{
  QSqlQuery q;
  q.prepare("SELECT SampleId FROM Sample Order By SampleId DESC Limit 1;");
  return q;
}


/*
 * Retrieves the path of all files associated to a given user
 *
 * The query contains 4 placeholders:
 * 	:userid (maps to Sample::UserId)
 */
QSqlQuery SSCQueries::getSamplePaths()
{
  QSqlQuery q;
  q.prepare("SELECT Path FROM Sample WHERE UserId = :userid;");
  return q;
}
