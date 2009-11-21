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

#ifndef SSCQUERIES_H
#define SSCQUERIES_H

#include <QSqlQuery>

/**
 * \class SSCQueries
 * \brief Base query provider
 *
 * This is the base class for all query provider classes. It contains
 * standard compliant sql queries which should work for most databases.
 * In case an adaption is needed, you can subclass this class and 
 * override the queries in question through dynamic binding.
 */
class SSCQueries
{
	public:
		SSCQueries() {}
		virtual ~SSCQueries() {}

		virtual QSqlQuery getUser();
		virtual QSqlQuery getUsers(bool includeUserId, bool includeSurname,
				bool includeGivenName, bool includeSex, bool includeBirthYear,
				bool includeZipcode, bool includeEducation, bool includeCurrentOccupation,
				bool includeMotherTongue, bool includeDiagnosis, bool includeOrientation,
				bool includeMotorfunction, bool includeCommunication, bool includeMouthMotoric,
				bool includeInterviewPossible, bool includeRepeatingPossible, bool includeInstitutionId,
				bool includeReferenceId);

		virtual QSqlQuery addUser();
		virtual QSqlQuery modifyUser();
		virtual QSqlQuery removeUser();

		virtual QSqlQuery getLanguages();

		virtual QSqlQuery getInstitutions();
		virtual QSqlQuery addInstitution();
		virtual QSqlQuery modifyInstitution();
		virtual QSqlQuery removeInstitution();
};

#endif

