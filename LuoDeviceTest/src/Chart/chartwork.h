#ifndef CHARTWORK_H
#define CHARTWORK_H

#include "qcustomplot.h"
#include "../ConstData.h"
#include <QTimer>
class ChartWork : public QCustomPlot
{
    Q_OBJECT
public:
    explicit ChartWork(QWidget* parent = nullptr);

    void AddPoints(QVector<float> ch_data);

    void SetXRange(double x_range);

    void SetChIndex(int index);

    void SetSamplingFrqe(SamplingFreq freq);
private:
    quint64 m_current_frame = 0;
    QVector<QVector<double>> y;
    QVector<double> x;

    double m_x_range = 5;
    int m_ch_plot_index = 0;
    SamplingFreq m_freq = SamplingFreq::Freq1000;

    QTimer m_update_timer;
    void UpdataData();

signals:

};

#endif // CHARTWORK_H
