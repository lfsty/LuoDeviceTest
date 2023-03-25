#include "filtercontroller.h"
#include "FilterData/FilterData_1000Hz.h"
#include "FilterData/FilterData_500Hz.h"
#include "FilterData/FilterData_250Hz.h"

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
            m_filter_bank.AddFilter(2, FilterData1000::HP0_1::NUM, FilterData1000::HP0_1::DEN);
            m_filter_bank.AddFilter(2, FilterData1000::Notch50::NUM, FilterData1000::Notch50::DEN);
            break;
        case Freq500:
            m_filter_bank.AddFilter(2, FilterData500::HP0_1::NUM, FilterData500::HP0_1::DEN);
            m_filter_bank.AddFilter(2, FilterData500::Notch50::NUM, FilterData500::Notch50::DEN);
            break;
        case Freq250:
            m_filter_bank.AddFilter(2, FilterData250::HP0_1::NUM, FilterData250::HP0_1::DEN);
            m_filter_bank.AddFilter(2, FilterData250::Notch50::NUM, FilterData250::Notch50::DEN);
            break;
        default:
            break;
    }
}

double FilterController::DoFilter(double data)
{
    return m_filter_bank.DoFilter(data);
}
