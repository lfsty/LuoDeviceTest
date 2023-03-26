#include "filterwork.h"
#include "FilterData/FilterData_1000Hz.h"
#include "FilterData/FilterData_500Hz.h"
#include "FilterData/FilterData_250Hz.h"

filterWork::filterWork(int ChNum, QObject* parent)
    : QObject{parent}
{
    m_ch_num = ChNum;

    m_vect_filterBank.resize(ChNum);
    for(int i = 0; i < ChNum; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m_vect_filterBank[i].AddFilter(FilterData1000::lowpass::fc15::NL - 1, FilterData1000::lowpass::fc15::NUM, FilterData1000::lowpass::fc15::DEN);
        }
    }
}

void filterWork::SetSamplingFreq(SamplingFreq samp_freq)
{
    m_freq_type = samp_freq;
}

void filterWork::DoFilter(QVector<float> data)
{
    if(m_is_filter_on)
    {

        QVector<float> ret_data;
        for(int i = 0; i < m_ch_num; i++)
        {
            ret_data.push_back(m_vect_filterBank[i].DoFilter(data[i]));
        }
        emit sig_Filter_output(ret_data);
    }
    else
    {
        emit sig_Filter_output(data);
    }
}

void filterWork::OnFilterEnabled(bool enable)
{
    m_is_filter_on = enable;
}

void filterWork::OnFilterLowPassEnalbed(bool enable)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        iter->m_filter_bank[0].SetEnabled(enable);
    }
}

void filterWork::OnFilterHighPassEnabled(bool enable)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        iter->m_filter_bank[1].SetEnabled(enable);
    }
}

void filterWork::OnFilterNotchEnabled(bool enable)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        iter->m_filter_bank[2].SetEnabled(enable);
    }
}

void filterWork::OnFilterHighPassImpenEnabled(bool enable)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        iter->m_filter_bank[3].SetEnabled(enable);
    }
}

void filterWork::OnFilterLowPassChanged(int index)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        bool _old_enabled = iter->m_filter_bank[0].isEnable();
        switch(m_freq_type)
        {
            case SamplingFreq::Freq1000:
                {
                    switch(index)
                    {
                        case FilterLowpass::fc15:
                            {
                                iter->m_filter_bank[0] = Filter<double>(FilterData1000::lowpass::fc15::NL - 1, FilterData1000::lowpass::fc15::NUM, FilterData1000::lowpass::fc15::DEN);
                                break;
                            }
                        case FilterLowpass::fc30:
                            {
                                iter->m_filter_bank[0] = Filter<double>(FilterData1000::lowpass::fc30::NL - 1, FilterData1000::lowpass::fc30::NUM, FilterData1000::lowpass::fc30::DEN);
                                break;
                            }
                        case FilterLowpass::fc55:
                            {
                                iter->m_filter_bank[0] = Filter<double>(FilterData1000::lowpass::fc55::NL - 1, FilterData1000::lowpass::fc55::NUM, FilterData1000::lowpass::fc55::DEN);
                                break;
                            }
                        case FilterLowpass::fc70:
                            {
                                iter->m_filter_bank[0] = Filter<double>(FilterData1000::lowpass::fc70::NL - 1, FilterData1000::lowpass::fc70::NUM, FilterData1000::lowpass::fc70::DEN);
                                break;
                            }
                        case FilterLowpass::fc100:
                            {
                                iter->m_filter_bank[0] = Filter<double>(FilterData1000::lowpass::fc100::NL - 1, FilterData1000::lowpass::fc100::NUM, FilterData1000::lowpass::fc100::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq500:
                {
                    switch(index)
                    {
                        case FilterLowpass::fc15:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq500::lowpass::fc15::NL - 1, Freq500::lowpass::fc15::NUM, Freq500::lowpass::fc15::DEN);
                                break;
                            }
                        case FilterLowpass::fc30:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq500::lowpass::fc30::NL - 1, Freq500::lowpass::fc30::NUM, Freq500::lowpass::fc30::DEN);
                                break;
                            }
                        case FilterLowpass::fc55:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq500::lowpass::fc55::NL - 1, Freq500::lowpass::fc55::NUM, Freq500::lowpass::fc55::DEN);
                                break;
                            }
                        case FilterLowpass::fc70:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq500::lowpass::fc70::NL - 1, Freq500::lowpass::fc70::NUM, Freq500::lowpass::fc70::DEN);
                                break;
                            }
                        case FilterLowpass::fc100:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq500::lowpass::fc100::NL - 1, Freq500::lowpass::fc100::NUM, Freq500::lowpass::fc100::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq250:
                {
                    switch(index)
                    {
                        case FilterLowpass::fc15:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq250::lowpass::fc15::NL - 1, Freq250::lowpass::fc15::NUM, Freq250::lowpass::fc15::DEN);
                                break;
                            }
                        case FilterLowpass::fc30:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq250::lowpass::fc30::NL - 1, Freq250::lowpass::fc30::NUM, Freq250::lowpass::fc30::DEN);
                                break;
                            }
                        case FilterLowpass::fc55:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq250::lowpass::fc55::NL - 1, Freq250::lowpass::fc55::NUM, Freq250::lowpass::fc55::DEN);
                                break;
                            }
                        case FilterLowpass::fc70:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq250::lowpass::fc70::NL - 1, Freq250::lowpass::fc70::NUM, Freq250::lowpass::fc70::DEN);
                                break;
                            }
                        case FilterLowpass::fc100:
                            {
//                                iter->m_filter_bank[0] = Filter<double>(Freq250::lowpass::fc100::NL - 1, Freq250::lowpass::fc100::NUM, Freq250::lowpass::fc100::DEN);
                                break;
                            }
                    }
                    break;
                }
        }
        iter->m_filter_bank[0].SetEnabled(_old_enabled);
    }
}

void filterWork::OnFilterHighPassChanged(int index)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        bool _old_enabled = iter->m_filter_bank[1].isEnable();
        switch(m_freq_type)
        {
            case SamplingFreq::Freq1000:
                {
                    switch(index)
                    {
                        case FilterHighpass::fc0_3:
                            {
                                iter->m_filter_bank[1] = Filter<double>(FilterData1000::highpass::fc0_3::NL - 1, FilterData1000::highpass::fc0_3::NUM, FilterData1000::highpass::fc0_3::DEN);
                                break;
                            }
                        case FilterHighpass::fc0_5:
                            {
                                iter->m_filter_bank[1] = Filter<double>(FilterData1000::highpass::fc0_5::NL - 1, FilterData1000::highpass::fc0_5::NUM, FilterData1000::highpass::fc0_5::DEN);
                                break;
                            }
                        case FilterHighpass::fc1:
                            {
                                iter->m_filter_bank[1] = Filter<double>(FilterData1000::highpass::fc1::NL - 1, FilterData1000::highpass::fc1::NUM, FilterData1000::highpass::fc1::DEN);
                                break;
                            }
                        case FilterHighpass::fc3:
                            {
                                iter->m_filter_bank[1] = Filter<double>(FilterData1000::highpass::fc3::NL - 1, FilterData1000::highpass::fc3::NUM, FilterData1000::highpass::fc3::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq500:
                {
                    switch(index)
                    {
                        case FilterHighpass::fc0_3:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq500::highpass::fc0_3::NL - 1, Freq500::highpass::fc0_3::NUM, Freq500::highpass::fc0_3::DEN);
                                break;
                            }
                        case FilterHighpass::fc0_5:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq500::highpass::fc0_5::NL - 1, Freq500::highpass::fc0_5::NUM, Freq500::highpass::fc0_5::DEN);
                                break;
                            }
                        case FilterHighpass::fc1:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq500::highpass::fc1::NL - 1, Freq500::highpass::fc1::NUM, Freq500::highpass::fc1::DEN);
                                break;
                            }
                        case FilterHighpass::fc3:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq500::highpass::fc3::NL - 1, Freq500::highpass::fc3::NUM, Freq500::highpass::fc3::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq250:
                {
                    switch(index)
                    {
                        case FilterHighpass::fc0_3:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq250::highpass::fc0_3::NL - 1, Freq250::highpass::fc0_3::NUM, Freq250::highpass::fc0_3::DEN);
                                break;
                            }
                        case FilterHighpass::fc0_5:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq250::highpass::fc0_5::NL - 1, Freq250::highpass::fc0_5::NUM, Freq250::highpass::fc0_5::DEN);
                                break;
                            }
                        case FilterHighpass::fc1:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq250::highpass::fc1::NL - 1, Freq250::highpass::fc1::NUM, Freq250::highpass::fc1::DEN);
                                break;
                            }
                        case FilterHighpass::fc3:
                            {
//                        iter->m_filter_bank[1] = Filter<double>(Freq250::highpass::fc3::NL - 1, Freq250::highpass::fc3::NUM, Freq250::highpass::fc3::DEN);
                                break;
                            }
                    }
                    break;
                }
        }

        iter->m_filter_bank[1].SetEnabled(_old_enabled);
    }
}

void filterWork::OnFilterNotchChanged(int index)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        bool _old_enabled = iter->m_filter_bank[2].isEnable();
        switch(m_freq_type)
        {
            case SamplingFreq::Freq1000:
                {
                    switch(index)
                    {
                        case FilterNotch::fc50:
                            {
                                iter->m_filter_bank[2] = Filter<double>(FilterData1000::notch::fc50::NL - 1, FilterData1000::notch::fc50::NUM, FilterData1000::notch::fc50::DEN);
                                break;
                            }
                        case FilterNotch::fc60:
                            {
                                iter->m_filter_bank[2] = Filter<double>(FilterData1000::notch::fc60::NL - 1, FilterData1000::notch::fc60::NUM, FilterData1000::notch::fc60::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq500:
                {
                    switch(index)
                    {
                        case FilterNotch::fc50:
                            {
//                        iter->m_filter_bank[2] = Filter<double>(Freq500::notch::fc50::NL - 1, Freq500::notch::fc50::NUM, Freq500::notch::fc50::DEN);
                                break;
                            }
                        case FilterNotch::fc60:
                            {
//                        iter->m_filter_bank[2] = Filter<double>(Freq500::notch::fc60::NL - 1, Freq500::notch::fc60::NUM, Freq500::notch::fc60::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq250:
                {
                    switch(index)
                    {
                        case FilterNotch::fc50:
                            {
//                        iter->m_filter_bank[2] = Filter<double>(Freq250::notch::fc50::NL - 1, Freq250::notch::fc50::NUM, Freq250::notch::fc50::DEN);
                                break;
                            }
                        case FilterNotch::fc60:
                            {
//                        iter->m_filter_bank[2] = Filter<double>(Freq250::notch::fc60::NL - 1, Freq250::notch::fc60::NUM, Freq250::notch::fc60::DEN);
                                break;
                            }
                    }
                    break;
                }
        }

        iter->m_filter_bank[2].SetEnabled(_old_enabled);
    }
}

void filterWork::OnFilterHighPassImpenChanged(int index)
{
    for(auto iter = m_vect_filterBank.begin(); iter != m_vect_filterBank.end(); iter++)
    {
        bool _old_enabled = iter->m_filter_bank[3].isEnable();
        switch(m_freq_type)
        {
            case SamplingFreq::Freq1000:
                {
                    switch(index)
                    {
                        case FilterHighpassImpen::fc50:
                            {
                                iter->m_filter_bank[3] = Filter<double>(FilterData1000::highpass::fc50::NL - 1, FilterData1000::highpass::fc50::NUM, FilterData1000::highpass::fc50::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc100:
                            {
                                iter->m_filter_bank[3] = Filter<double>(FilterData1000::highpass::fc100::NL - 1, FilterData1000::highpass::fc100::NUM, FilterData1000::highpass::fc100::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc150:
                            {
                                iter->m_filter_bank[3] = Filter<double>(FilterData1000::highpass::fc150::NL - 1, FilterData1000::highpass::fc150::NUM, FilterData1000::highpass::fc150::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc200:
                            {
                                iter->m_filter_bank[3] = Filter<double>(FilterData1000::highpass::fc200::NL - 1, FilterData1000::highpass::fc200::NUM, FilterData1000::highpass::fc200::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq500:
                {
                    switch(index)
                    {
                        case FilterHighpassImpen::fc50:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq500::highpass::fc50::NL - 1, Freq500::highpass::fc50::NUM, Freq500::highpass::fc50::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc100:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq500::highpass::fc100::NL - 1, Freq500::highpass::fc100::NUM, Freq500::highpass::fc100::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc150:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq500::highpass::fc150::NL - 1, Freq500::highpass::fc150::NUM, Freq500::highpass::fc150::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc200:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq500::highpass::fc200::NL - 1, Freq500::highpass::fc200::NUM, Freq500::highpass::fc200::DEN);
                                break;
                            }
                    }
                    break;
                }
            case SamplingFreq::Freq250:
                {
                    switch(index)
                    {
                        case FilterHighpassImpen::fc50:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq250::highpass::fc50::NL - 1, Freq250::highpass::fc50::NUM, Freq250::highpass::fc50::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc100:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq250::highpass::fc100::NL - 1, Freq250::highpass::fc100::NUM, Freq250::highpass::fc100::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc150:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq250::highpass::fc150::NL - 1, Freq250::highpass::fc150::NUM, Freq250::highpass::fc150::DEN);
                                break;
                            }
                        case FilterHighpassImpen::fc200:
                            {
//                        iter->m_filter_bank[3] = Filter<double>(Freq250::highpass::fc200::NL - 1, Freq250::highpass::fc200::NUM, Freq250::highpass::fc200::DEN);
                                break;
                            }
                    }
                    break;
                }
        }
        iter->m_filter_bank[3].SetEnabled(_old_enabled);
    }
}
