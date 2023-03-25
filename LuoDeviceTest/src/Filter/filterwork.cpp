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
    if(m_is_filter_on)
    {

        QVector<float> ret_data;
        for(int i = 0; i < m_ch_num; i++)
        {
            ret_data.push_back(m_vect_filtercontroller[i].DoFilter(data[i]));
        }

        emit sig_Filter_output(ret_data);
    }
    else
    {
        emit sig_Filter_output(data);
    }
}

void filterWork::SetFilterEnabled(bool filter_enable)
{
    m_is_filter_on = filter_enable;
}

void filterWork::OnFilterEnabled(bool enable)
{

}

void filterWork::OnFilterLowPassEnalbed(bool enable)
{

}

void filterWork::OnFilterHighPassEnabled(bool enable)
{

}

void filterWork::OnFilterNotchEnabled(bool enable)
{

}

void filterWork::OnFilterHighPassImpenEnabled(bool enable)
{

}

void filterWork::OnFilterLowPassChanged(int index)
{

}

void filterWork::OnFilterHighPassChanged(int index)
{

}

void filterWork::OnFilterNotchChanged(int index)
{

}

void filterWork::OnFilterHighPassImpenChanged(int index)
{

}
