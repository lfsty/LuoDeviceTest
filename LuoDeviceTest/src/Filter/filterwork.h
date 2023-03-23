#ifndef FILTERWORK_H
#define FILTERWORK_H

#include <QObject>
#include "filtercontroller.h"
#include "../ConstData.h"

class filterWork : public QObject
{
    Q_OBJECT
public:
    explicit filterWork(int ChNum, QObject* parent = nullptr);
    void SetSamplingFreq(SamplingFreq samp_freq);
    void DoFilter(QVector<float> data);

private:
    int m_ch_num = TOTAL_CH_NUM;
    QVector<FilterController> m_vect_filtercontroller;

signals:
    void sig_Filter_output(QVector<float>);
};

#endif // FILTERWORK_H
