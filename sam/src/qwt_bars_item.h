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

#ifndef qwt_bars_item_IS_INCLUDED     // -*- C++ -*-
#define qwt_bars_item_IS_INCLUDED

#include <qwt_plot_item.h>
#include <qwt_scale_draw.h>
#include <qwt_series_data.h>


#include <QStringList>

/*
 * QwtBarsItem
 */
class QwtBarsItem : public QwtPlotItem {

public:
	enum { 
		Rtti_PlotBars = Rtti_PlotUserItem,
		Rtti_PlotBarSerie 
	};
	QwtBarsItem();
	virtual ~QwtBarsItem();

	virtual int rtti() const;

	void addSerie( const QString& text, QwtSeriesData<double>& data, const QBrush& brush, const QPen& pen );

//    virtual QwtDoubleRect boundingRect() const;
	virtual QWidget *legendItem() const;

	virtual void draw(QPainter *, const QwtScaleMap &xMap, 
					  const QwtScaleMap &yMap, const QRectF &) const;

	void setBaseline(double reference);
	double baseline() const;

	enum Type {
		SideBySide,
		Stacked,
		Grouped
	};
	void setType( Type t );
	Type type() const;

	void setBarsFraction( double f );
	double barsFraction();

	
	enum BarsAttribute
	{
		Auto = 0,
		Xfy = 1
	};
	void setBarsAttribute(BarsAttribute, bool on = true);
	bool testBarsAttribute(BarsAttribute) const;

	void remove( QwtPlotItem* );
private:
	void updateSeries();
	
	class PrivateData;
	PrivateData *d;
};
/*
 * QwtScaleDrawLabels
 */
class QwtScaleDrawLabels : public QwtScaleDraw {
public:
	QwtScaleDrawLabels( const QStringList& l = QStringList(), int indexOffset = 0 );
	virtual QwtText label(double val) const;
private:
	QStringList d_labels;
	int d_indexOffset;
};

#endif

