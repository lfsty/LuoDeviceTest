#include "chartwork.h"

ChartWork::ChartWork(QWidget* parent)
    : QCustomPlot{parent}
{
    for(int i = 0; i < TOTAL_CH_NUM; i++)
    {
        addGraph();
        y.append(QVector<double> {});
    }
    m_update_timer.setInterval(20);
    connect(&m_update_timer, &QTimer::timeout, this, &ChartWork::UpdataData);
    m_update_timer.start();
}

void ChartWork::AddPoints(QVector<float> ch_data)
{
    if(x.size() > m_x_range * m_freq)
    {
        x.pop_front();
        for(int i = 0; i < TOTAL_CH_NUM; i++)
        {
            y[i].pop_front();
        }
    }

    x.append(double(m_current_frame++) / m_freq);
    for(int i = 0; i < TOTAL_CH_NUM; i++)
    {
        y[i].append(ch_data[i]);
    }
}

void ChartWork::SetXRange(double x_range)
{
    m_x_range = x_range;
    replot();
}

void ChartWork::SetChIndex(int index)
{
    m_ch_plot_index = index;

    for(int i = 0; i < TOTAL_CH_NUM; i++)
    {
        graph(i)->data().data()->clear();
    }
}

void ChartWork::SetSamplingFrqe(SamplingFreq freq)
{
    m_freq = freq;
}

void ChartWork::UpdataData()
{
    if(x.isEmpty())
    {
        return;
    }
    graph(m_ch_plot_index)->setData(x, y[m_ch_plot_index]);
    //设置坐标轴标签
//    xAxis->setLabel("x");
//    yAxis->setLabel("y");
    double x_begin = x.size() == 0 ? 0 : x[0];
    xAxis->setRange(x_begin, x_begin + m_x_range);

    float _y_min = *std::min_element(std::begin(y[m_ch_plot_index]), std::end(y[m_ch_plot_index]));
    float _y_max = *std::max_element(std::begin(y[m_ch_plot_index]), std::end(y[m_ch_plot_index]));
    float _range = (_y_max - _y_min) * 0.125;
    yAxis->setRange(_y_min - _range, _y_max + _range );

    replot();
}

