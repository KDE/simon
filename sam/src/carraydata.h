/*
 *  Copyright (C) 2007 Knut Finstad <knut.finstad@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_CARRAYDATA_H_4002119636CC42C68FE07273F9000A73
#define SIMON_CARRAYDATA_H_4002119636CC42C68FE07273F9000A73

#include <qwt_series_data.h>
#include <qwt_compat.h>

template <typename T>
class CArrayData : public QwtSeriesData<T>
{
public:
	CArrayData( const T* v, int size ) : d_v( v ) ,d_size( size ) {}
	virtual QwtSeriesData<T> *copy() const {
		return new CArrayData( d_v, d_size );
	}
	virtual size_t size() const { return d_size; }
	virtual T sample(size_t i) const { return d_v[i]; }

	virtual QwtDoubleRect boundingRect() const {
		return QwtDoubleRect();
	}
private:
	const T* d_v;
	int d_size;

};
#endif
