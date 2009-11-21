/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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
 * Returns all available institutions
 *
 * Institution::InstitutionId
 * Institution::Name
 */
QSqlQuery SSCQueries::getInstitutions()
{
	QSqlQuery q("SELECT InstitutionId, Name from Institution");
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




