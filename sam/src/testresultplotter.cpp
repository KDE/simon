/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "testresultplotter.h"
#include "testresultwidget.h"
#include "qwt_bars_item.h"
#include "carraydata.h"
#include "qwt_series_data.h"
#include <qwt_plot.h>
#include <qwt_legend.h>
//#include <qwt_legend_item.h>
#include <QStringList>

#if QWT_VERSION >= 0x060100
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>
#endif

void TestResultPlotter::plot(const QStringList& labels, const QList<double>& confidence, const QList<double>& accuracy, 
	    QwtPlot *plot, QwtBarsItem *barGraph, QwtLegend *barGraphLegend)
{
  int count = labels.count();
  Q_ASSERT(count == confidence.count());
  Q_ASSERT(count == accuracy.count());
  
  #if QWT_VERSION < 0x060100
  double *accuracyA = new double[count];
  double *confidenceA = new double[count];
  for (int i=0; i < count; i++)
  {
    confidenceA[i] = confidence[i];
    accuracyA[i] = accuracy[i];
  }
  
  QwtSeriesData<double> *accSeries = new CArrayData<double>( accuracyA, count );
  barGraph->addSerie( i18n("Accuracy"), *accSeries, QBrush( Qt::blue ), QPen( QColor( Qt::darkBlue ), 1 ) );

  QwtSeriesData<double> *confSeries = new CArrayData<double>( confidenceA, count );
  barGraph->addSerie( i18n("Confidence"), *confSeries, QBrush( Qt::green ), QPen( QColor( Qt::darkGreen ), 1 ) );

  barGraph->updateLegend(barGraphLegend);
  barGraph->setType(QwtBarsItem::SideBySide);
  #else
  static const char *colors[] = { "Blue", "Red" };
  const int numBars = 2; // accuracy, confidence
  QList<QwtText> barTitles;
  barTitles << i18n("Accuracy");
  barTitles << i18n("Confidence");
  barGraph->setBarTitles(barTitles);

  barGraph->setLegendIconSize(QSize(10, 14));

  for ( int i = 0; i < numBars; i++ )
  {
    QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setLineWidth(2);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setPalette(QPalette(colors[i]));
    barGraph->setSymbol(i, symbol);
  }

  QVector<QVector<double> > series;
  for (int i = 0; i < count; i++)
  {
    QVector<double> values;
    values << accuracy[i];
    values << confidence[i];
    series << values;
  }

  barGraph->setSamples(series);
  #endif

  barGraph->attach(plot);
  plot->setAxisScaleDraw(QwtPlot::xBottom, new QwtScaleDrawLabels( labels, 1 ));
  
  // scale:
  #if QWT_VERSION < 0x60100
  plot->setAxisMaxMinor(QwtPlot::xBottom, 0);
  plot->setAxisMaxMajor(QwtPlot::xBottom, count+1);
  #else
  QwtScaleDraw *scaleDraw1 = new QwtScaleDrawLabels( labels, 0 );
  scaleDraw1->enableComponent(QwtScaleDraw::Labels, true);
  scaleDraw1->enableComponent(QwtScaleDraw::Backbone, false);
  scaleDraw1->enableComponent(QwtScaleDraw::Ticks, false);
  plot->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw1);

  QwtScaleDraw *scaleDraw2 = plot->axisScaleDraw(QwtPlot::yLeft);
  scaleDraw2->enableComponent(QwtScaleDraw::Backbone, true);
  scaleDraw2->enableComponent(QwtScaleDraw::Labels, true);
  scaleDraw2->enableComponent(QwtScaleDraw::Ticks, true);

  plot->plotLayout()->setAlignCanvasToScale(QwtPlot::xBottom, true);
  plot->plotLayout()->setAlignCanvasToScale(QwtPlot::yLeft, false);
  #endif

  plot->replot();
}

void TestResultPlotter::plot(const QList<TestResultWidget*>& testResults, QwtPlot *plot, QwtBarsItem *barGraph, QwtLegend *barGraphLegend)
{
  QStringList labels;
  QList<double> confidence;
  QList<double> accuracy;
  
  foreach (TestResultWidget *test, testResults)
  {
    if (test->getState() != TestResultWidget::Done)
      continue;
    confidence << test->getConfidence() * 100.0;
    accuracy << test->getAccuracy() * 100.0;

    labels << test->getTag();
  }
  TestResultPlotter::plot(labels, confidence, accuracy, plot, barGraph, barGraphLegend);
}


