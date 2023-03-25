#ifndef FILTERWORK_H
#define FILTERWORK_H

#include <QObject>
#include "filtercontroller.h"
#include "../ConstData.h"
namespace FilterLowpass
{
    enum FilterLowpass
    {
        fc15 = 0,
        fc30,
        fc55,
        fc70,
        fc100
    };
};
namespace FilterHighpass
{
    enum FilterHighpass
    {
        fc0_3 = 0,
        fc0_5,
        fc1,
        fc3
    };
}
namespace FilterNotch
{
    enum FilterNotch
    {
        fc50 = 0,
        fc60
    };
}
namespace FilterHighpassImpen
{
    enum FilterHighpassImpen
    {
        fc50 = 0,
        fc100,
        fc150,
        fc200
    };
}


class filterWork : public QObject
{
    Q_OBJECT
public:
    explicit filterWork(int ChNum, QObject* parent = nullptr);
    void SetSamplingFreq(SamplingFreq samp_freq);
    void DoFilter(QVector<float> data);
    void SetFilterEnabled(bool filter_enable);

private:
    int m_ch_num = TOTAL_CH_NUM;
    QVector<FilterController> m_vect_filtercontroller;
    bool m_is_filter_on = false;

public:
    void OnFilterEnabled(bool enable);
    void OnFilterLowPassEnalbed(bool enable);
    void OnFilterHighPassEnabled(bool enable);
    void OnFilterNotchEnabled(bool enable);
    void OnFilterHighPassImpenEnabled(bool enable);

    void OnFilterLowPassChanged(int index);
    void OnFilterHighPassChanged(int index);
    void OnFilterNotchChanged(int index);
    void OnFilterHighPassImpenChanged(int index);

signals:
    void sig_Filter_output(QVector<float>);
};

#endif // FILTERWORK_H
