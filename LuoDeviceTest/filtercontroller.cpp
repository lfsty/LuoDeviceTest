#include "filtercontroller.h"

FilterController::FilterController()
{

}

void FilterController::SetSamplingFreq(SamplingFreq sampling_freq)
{
    m_filter_bank.clear();
    m_freq_type = sampling_freq;
    switch (m_freq_type)
    {
        case Freq1000:
            m_filter_bank.AddFilter(2, FilterData1000::HP0_1::DEN, FilterData1000::HP0_1::NUM);
            m_filter_bank.AddFilter(2, FilterData1000::Notch50::DEN, FilterData1000::Notch50::NUM);
            break;
        case Freq500:
            m_filter_bank.AddFilter(2, FilterData500::HP0_1::DEN, FilterData500::HP0_1::NUM);
            m_filter_bank.AddFilter(2, FilterData500::Notch50::DEN, FilterData500::Notch50::NUM);
            break;
        case Freq250:
            m_filter_bank.AddFilter(2, FilterData250::HP0_1::DEN, FilterData250::HP0_1::NUM);
            m_filter_bank.AddFilter(2, FilterData250::Notch50::DEN, FilterData250::Notch50::NUM);
            break;
        default:
            break;
    }
}

double FilterController::DoFilter(double data)
{
    return m_filter_bank.DoFilter(data);
}
