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

#include "qwt_bars_item.h"

#include <qwt_plot_seriesitem.h>
#include <qwt_painter.h>
#include <qwt_compat.h>
#include <qwt_legend_item.h>
#include <qwt_plot.h>
#include <qwt_scale_map.h>

#include <QDebug>
#include <QPen>
#include <QPainter>
#include <KDebug>

class QwtBarsItem::PrivateData {
public:
	int attributes;
	double reference;
	double barsFraction;
	Type type;

	class BarSerie : public QwtPlotSeriesItem<double> {
	public:
		BarSerie( QwtBarsItem* parent,
				  const QString& text, QwtSeriesData<double>& data,
				  const QBrush& brush, const QPen& pen )
			: QwtPlotSeriesItem<double>( text ), d_parent( parent ),
			  d_brush( brush ),
			  d_pen( pen ), d_rects( d_values.size() ) {
			setData( &data );

			setItemAttribute(QwtPlotItem::AutoScale, true);
			setItemAttribute(QwtPlotItem::Legend, true);
		}
		~BarSerie() {
			if ( d_parent ) {
				d_parent->remove( this );
			}
		}
		int rtti() const { return Rtti_PlotBarSerie; }
		void setParent( QwtBarsItem* p ) { d_parent = p; }
//		const QwtArray<double>& values() { return d_values; }
		void setRects( const QwtArray<QRectF>& rects ) {
			d_rects = rects;
			QwtDoubleRect b;
			if ( !d_rects.isEmpty() ) {
				b = d_rects.front();
				for ( int i = 1; i < d_rects.size(); ++i ) {
					b |= d_rects[i];
				}
			}
			d_bounding = b;
			itemChanged();
		}
		virtual QwtDoubleRect boundingRect() const {
			return d_bounding;
		}
		virtual void draw(QPainter * p, const QwtScaleMap &xMap, 
						  const QwtScaleMap &yMap, const QRectF & rect) const {
      drawSeries(p, xMap, yMap, rect, 0, d_rects.size());
		}

    virtual void drawSeries( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &/*canvasRect*/, int from, int to ) const {
			p->save();
			p->setBrush( d_brush );
			p->setPen( d_pen );
			QPolygon polygon( 4 );
			for ( int i = from; i < to; ++i ) {
				int x = xMap.transform( d_rects[i].x() );
				int y = yMap.transform( d_rects[i].y() );
				int x2 = xMap.transform( d_rects[i].x()+d_rects[i].width() );
				int y2 = yMap.transform( d_rects[i].y()+d_rects[i].height() );
				polygon.setPoint( 0, x, y );
				polygon.setPoint( 1, x2, y );
				polygon.setPoint( 2, x2, y2 );
				polygon.setPoint( 3, x, y2 );

				QwtPainter::drawPolygon( p, polygon );
			}
			p->restore();
    }

		QWidget* legendItem() const {
			return new QwtLegendItem;
		}
		void updateLegend(QwtLegend * legend ) const {
			if ( !legend )
				return;

			QwtPlotItem::updateLegend(legend);

			QwtLegendItem *legendItem = dynamic_cast<QwtLegendItem*>( legend->find(this) );
			if ( !legendItem ) 
				return;

#if QT_VERSION < 0x040000
			const bool doUpdate = legendItem->isUpdatesEnabled();
#else
			const bool doUpdate = legendItem->updatesEnabled();
#endif
			legendItem->setUpdatesEnabled(false);

      if ( !title().isEmpty() )
      {
        legendItem->setText(title());
      } else {
        legendItem->setText(QwtText());
      }

      QPixmap p(8,8);
      QPainter painter(&p);
      painter.setBrush(d_brush);
      painter.setPen(d_pen);
      painter.drawRect(0,0,8,8);
      legendItem->setIdentifier(p);

			legendItem->setUpdatesEnabled(doUpdate);
			legendItem->update();
		}

	private:
		QwtBarsItem* d_parent;
		QwtArray<double> d_values;
		QBrush d_brush;
		QPen d_pen;
		QwtArray<QRectF> d_rects;
		QwtDoubleRect d_bounding;
	};

	QList<BarSerie*> series;
};

QwtBarsItem::QwtBarsItem()
{
	d = new PrivateData;

	d->reference = 0.0;
	d->attributes = QwtBarsItem::Auto;
	d->type = SideBySide;
	d->barsFraction = 0.8;

	setItemAttribute(QwtPlotItem::AutoScale, true);
	setItemAttribute(QwtPlotItem::Legend, false);
}

QwtBarsItem::~QwtBarsItem()
{
	for ( int i = 0; i < d->series.size(); ++i ) {
		d->series[i]->setParent( 0 );
	}
	delete d;
}
int QwtBarsItem::rtti() const { return Rtti_PlotBars; }	

void QwtBarsItem::addSerie( const QString& text, QwtSeriesData<double>& data,
							const QBrush& brush, const QPen& pen )
{
	d->series.push_back( new PrivateData::BarSerie( this, text, data, brush, pen ) );
	if ( plot() ) {
		d->series.back()->attach( plot() );
	}
	updateSeries();

}
/*QwtDoubleRect QwtBarsItem::boundingRect() const
  {

  }*/
QWidget * QwtBarsItem::legendItem() const
{
	return 0;
}
void QwtBarsItem::setBaseline(double reference)
{
	if ( d->reference != reference )
	{
		d->reference = reference;
		itemChanged();
	}

}
double QwtBarsItem::baseline() const
{
	return d->reference;

}
void QwtBarsItem::setType( Type t )
{
	if ( d->type != t ) {
		d->type = t;
		updateSeries();
	}
}
QwtBarsItem::Type QwtBarsItem::type() const
{
	return d->type;
}
void QwtBarsItem::setBarsFraction( double f )
{
	if ( d->barsFraction != f ) {
		d->barsFraction = qBound( 0.1, f, 1.0 );
		updateSeries();
	}
}
double QwtBarsItem::barsFraction()
{
	return d->barsFraction;
}
void QwtBarsItem::setBarsAttribute(BarsAttribute attribute , bool on )
{
	if ( bool(d->attributes & attribute) == on )
		return;

	if ( on )
		d->attributes |= attribute;
	else
		d->attributes &= ~attribute;

	updateSeries();
}
bool QwtBarsItem::testBarsAttribute(BarsAttribute attribute ) const
{
	return d->attributes & attribute;

}

void QwtBarsItem::updateSeries()
{
	if ( !plot() ) return;
	// first check that they are all still present:
	QList<PrivateData::BarSerie*> curr;
	foreach ( PrivateData::BarSerie* s, d->series ) {
		if ( plot()->itemList().contains( s ) ) {
			curr.push_back( s );
		}
	}

	const double barsFraction = d->barsFraction;
	switch ( d->type ) {
	case SideBySide: {
		const double singleBarWidth = barsFraction/curr.size();
		for ( int i = 0; i < curr.size(); ++i ) {
			PrivateData::BarSerie* s = curr[i];
			QwtArray<QwtDoubleRect> rects( s->dataSize() );
			for ( unsigned int v = 0; v < s->dataSize(); ++v ) {
				double x1 = v + 1-barsFraction/2.0 + i*singleBarWidth;
				double x2 = v + 1-barsFraction/2.0 + (i+1)*singleBarWidth;

				double y1 = d->reference;
				double y2 = s->sample(v);

				if ( y1 > y2 ) {
					qSwap( y1, y2 );
				}
				if ( d->attributes & Xfy ) {
					rects[v] = QwtDoubleRect( y2, -x2, y2-y1, x2-x1 );
				} else {
					rects[v] = QwtDoubleRect( x1, y1, x2-x1, y2-y1 );
				}
				qDebug() << rects[v];
			}
			s->setRects( rects );
		}
	}
		break;

	case Stacked: {
		const double barsWidth = barsFraction;
		int max = 0;
		foreach ( PrivateData::BarSerie* s, d->series ) {
			max = qMax( (int) s->dataSize(), max );
		}
		QwtArray<double> posTot( max, 0.0 ), negTot( max, 0.0 );
		// fill with d->reference?
		for ( int i = 0; i < curr.size(); ++i ) {
			PrivateData::BarSerie* s = curr[i];
			QwtArray<QwtDoubleRect> rects( s->dataSize() );
			for ( unsigned int v = 0; v < s->dataSize(); ++v ) {
				const double value = s->sample(v);
				double x1 = v + 1-barsFraction/2.0;
				double x2 = x1 + barsWidth;
				double y1, y2;
				if ( value >= 0 ) {
					y1 = posTot[v];
					y2 = y1 + value;
					posTot[v] = y2;
				} else {
					y1 = negTot[v];
					y2 = y1 + value;
					negTot[v] = y2;
				}
				if ( y1 > y2 ) {
					qSwap( y1, y2 );
				}
				if ( d->attributes & Xfy ) {
					rects[v] = QwtDoubleRect( y2, -x2, y2-y1, x2-x1 );
				} else {
					rects[v] = QwtDoubleRect( x1, y1, x2-x1, y2-y1 );
				}
				qDebug() << rects[v];
			}
			s->setRects( rects );
		}
	}
		break;
	case Grouped: {
		for ( int i = 0; i < curr.size(); ++i ) {
			PrivateData::BarSerie* s = curr[i];
			QwtArray<QwtDoubleRect> rects( s->dataSize() );
			const double singleBarWidth = barsFraction/s->dataSize();
			for ( unsigned int v = 0; v < s->dataSize(); ++v ) {
				double x1 = i + 1-barsFraction/2.0 + v*singleBarWidth;
				double x2 = i + 1-barsFraction/2.0 + (v+1)*singleBarWidth;

				double y1 = d->reference;
				double y2 = s->sample(v);

				if ( y1 > y2 ) {
					qSwap( y1, y2 );
				}
				if ( d->attributes & Xfy ) {
					rects[v] = QwtDoubleRect( y2, -x2, y2-y1, x2-x1 );
				} else {
					rects[v] = QwtDoubleRect( x1, y1, x2-x1, y2-y1 );
				}
			}
			s->setRects( rects );
		}
	}
		break;
	}
}

void QwtBarsItem::remove( QwtPlotItem* i )
{
	if ( PrivateData::BarSerie* s = dynamic_cast<PrivateData::BarSerie*>( i ) ) {
		d->series.removeAll( s );
	}
}
void QwtBarsItem::draw(QPainter *, const QwtScaleMap &, 
					   const QwtScaleMap &, const QRectF &) const
{

}


QwtScaleDrawLabels::QwtScaleDrawLabels( const QStringList& l, int indexOffset ) : d_labels( l ), d_indexOffset( indexOffset ) {}
QwtText QwtScaleDrawLabels::label(double val) const {
	// check if val is int: 
	const double ceilVal = ::ceil( val );
	if ( val != ceilVal ) {
		return QwtText();
	}
	int intVal = (int)val - d_indexOffset;
		
	if ( intVal < 0 || intVal >= d_labels.size() )
		return QwtText();
	return d_labels[intVal];
}


