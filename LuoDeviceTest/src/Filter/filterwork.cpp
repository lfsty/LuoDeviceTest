#include "filterwork.h"


filterWork::filterWork(int ChNum, QObject* parent)
    : QObject{parent}
{
    m_ch_num = ChNum;
    for(int i = 0; i < ChNum; i++)
    {
        m_vect_filtercontroller.append(FilterController());
    }
}

void filterWork::SetSamplingFreq(SamplingFreq samp_freq)
{
    for(auto iter = m_vect_filtercontroller.begin(); iter != m_vect_filtercontroller.end(); iter++)
    {
        iter->SetSamplingFreq(samp_freq);
    }
}

void filterWork::DoFilter(QVector<float> data)
{
    QVector<float> ret_data;
    for(int i = 0; i < m_ch_num; i++)
    {
        ret_data.push_back(m_vect_filtercontroller[i].DoFilter(data[i]));
    }

    emit sig_Filter_output(ret_data);
}
