#include "CXBarChart.h"
#include <QtCharts/QBarSet>
#include <QtCore/QTime>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE


int CXBarChart::s_step = 0;

CXBarChart::CXBarChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(0)
{
    qsrand((uint) QTime::currentTime().msec());

    initChart();

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateChart()));
    m_timer.setInterval(5000);
    m_timer.start();
}

CXBarChart::~CXBarChart()
{

}

//初始化条形图
void CXBarChart::initChart()
{
    int nValueMax = 10;//最大Y值
    int nSet = 5;//条形个数
    int nCat = 6;//类别数

    //! 初始化series
    m_series = new QStackedBarSeries(this);
    //    m_series->setBarWidth(0.5);//设置每个条形图的宽度（占大格的百分比），默认是0.5，0.5表示百分之五十
    m_series->setLabelsVisible(true);//显示标签,默认不显示
    //    m_series->setLabelsAngle(45);//标签旋转角度（单位：度）
    m_series->setLabelsFormat("Y:@value");//标签的内容和样式
    //    m_series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);

    //!create data for bar series
    for(int i0 = 0; i0 < nSet; i0++)
    {
        QBarSet *set = new QBarSet(QString("类%1").arg(i0));
        //可单独设置每个set的颜色，标签等选项，也可以不用设置，则会自动匹配相关颜色标签等
        //    set->setLabelColor(Qt::blue);//设置标签颜色
        //    set->setLabelBrush(QBrush(Qt::lightGray));
        //    set->setLabelFont(QFont("Serif"));
        //    set->setPen(QPen(Qt::green));//设置绘制边框的笔
        //    set->setColor(Qt::red);//设置颜色
        for(int i1 = 0; i1 < nCat; ++i1)
        {
            *set << qrand() % nValueMax;
        }
        m_series->append(set);
    }

    //add series
    addSeries(m_series);

    //create axis
    createDefaultAxes();

    //!create x axis
    for(int i1 = 0; i1 < nCat; ++i1)
        m_categories << QString("%1").arg(++s_step);
    m_axisX = new QBarCategoryAxis();
    m_axisX->append(m_categories);
    m_axisX->setTitleText("月份");//设置轴标题，默认为空
    //    m_axisX->setVisible(false);//是否显示该轴，默认显示
    //    m_axisX->setTitleBrush();
    //    m_axisX->setTitleFont();
    //    m_axisX->setTitleVisible(false);//
    //    m_axisX->setLineVisible(false);//是否显示刻度线
    //    m_axisX->setLinePen();
    //    m_axisX->setLinePenColor();
    //    m_axisX->setLabelsVisible(false);//是否显示标签，如 1月，2月，默认是显示
    //    m_axisX->setLabelsAngle();
    //    m_axisX->setLabelsBrush();
    //    m_axisX->setLabelsColor();
    //    m_axisX->setLabelsFont();
    //    m_axisX->setGridLineVisible(false);//是否显示网格线，默认显示
    //    m_axisX->setGridLineColor();
    //    m_axisX->setGridLinePen();
    //    m_axisX->setMax("6月");//设置最大的类别
    //    m_axisX->setMin("3月");//设置最小的类别
    //    m_axisX->setShadesVisible(true);//是否显示深浅对比（相邻两个类别分别以深浅两种颜色表示），默认否
    //    m_axisX->setShadesBorderColor(Qt::red);
    //    m_axisX->setShadesBrush();
    //    m_axisX->setShadesColor(Qt::darkBlue);//设置深色的颜色
    //    m_axisX->setShadesPen();
    setAxisX(m_axisX, m_series);//

    //! show the legend
    QLegend* lgd = legend();
//    lgd->setVisible(true);
    lgd->setAlignment(Qt::AlignTop);
    //    lgd->setBackgroundVisible(true);
    //    lgd->setAutoFillBackground(true);//默认为否
    //    lgd->setBorderColor(Qt::red);
    //    lgd->setBrush(QBrush(Qt::red));
    //    lgd->setColor(Qt::red);
    //    lgd->setLabelColor(Qt::red);
    //    lgd->setLabelBrush(QBrush(Qt::red));
    //    lgd->setLayoutDirection(Qt::RightToLeft);
    QFont ft = lgd->font();
//    ft.setBold(true);
    ft.setPointSizeF(10);
    lgd->setFont(ft);//设置字体
    lgd->setMarkerShape(QLegend::MarkerShapeCircle);//标记的形状

    //! set others
    setTitle("动态堆积柱状图");
    setAnimationOptions(QChart::SeriesAnimations);//设置动画选项，默认是没有动画
    setAnimationDuration(1000);//动画持续时间，单位：ms
}

void CXBarChart::updateChart()
{
    int nValueMax = 10;
//    if(m_series && m_series->barSets().count() > 0)
//    {
//        foreach (QBarSet* set, m_series->barSets())
//        {
//            if(set)
//            {
//                //删除所有数据
//                if(set->count() > 0)
//                    set->remove(0,set->count());
//                //添加新数据
//                for(int i1 = 0; i1 < 6; ++i1)
//                {
//                    set->append(qrand() % nValueMax);
//                }
//            }
//        }
//    }

    if(m_series && m_series->barSets().count() > 0)
    {
        foreach (QBarSet* set, m_series->barSets())
        {
            if(set)
            {
                if(set->count() > 0)
                {
                    set->remove(0);//删除开头的数据
                    set->append(qrand() % nValueMax);//在末尾添加新数据
                }
            }
        }
    }

    m_axisX->clear();
    m_categories.removeFirst();
    m_categories.append(QString("%1").arg(++s_step));
    m_axisX->append(m_categories);
}
