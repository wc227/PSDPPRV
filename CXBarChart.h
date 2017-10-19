#ifndef CXBARCHART_H
#define CXBARCHART_H

#include <QtCore/QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QStackedBarSeries>

QT_CHARTS_USE_NAMESPACE

/// 自定义的动态堆叠条形图
/// \brief The CXBarChart class
///
class CXBarChart: public QChart
{
    Q_OBJECT
public:
    CXBarChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~CXBarChart();

    void initChart();//初始化条形图

public slots:
    void updateChart();//更新条形图

private:
    QTimer m_timer;//定时器
    QStringList m_categories;
    QStackedBarSeries *m_series;
    QBarCategoryAxis *m_axisX;
};

#endif // CXBARCHART_H
