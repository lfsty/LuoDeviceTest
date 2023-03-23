#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H

#include "filterbank.h"
#include "../ConstData.h"
class FilterController
{
public:
    FilterController();
public:
    void SetSamplingFreq(SamplingFreq sampling_freq);
    double DoFilter(double data);
private:
    FilterBank<float> m_filter_bank;
    SamplingFreq m_freq_type = Freq1000;
};

#endif // FILTERCONTROLLER_H
