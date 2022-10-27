#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H

#include "filterbank.h"
#include "Filter_data/FilterData_1000Hz.h"
#include "Filter_data/FilterData_500Hz.h"
#include "Filter_data/FilterData_250Hz.h"
#include "ConstData.h"
class FilterController
{
public:
    FilterController();
public:
    void SetSamplingFreq(SamplingFreq sampling_freq);
    double DoFilter(double data);
private:
    FilterBank m_filter_bank;
    SamplingFreq m_freq_type = Freq1000;
};

#endif // FILTERCONTROLLER_H
