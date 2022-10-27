#include "filterbank.h"

FilterBank::FilterBank()
{

}

void FilterBank::AddFilter(Filter filter)
{
    m_filter_bank.push_back(filter);
}

void FilterBank::AddFilter(int N, const double* DEN, const double* NUM)
{
    Filter tmp_filter(N, DEN, NUM);
    AddFilter(tmp_filter);
}

double FilterBank::DoFilter(double data)
{
    double ret_data = data;
    for(auto iter = m_filter_bank.begin(); iter != m_filter_bank.end(); iter++)
    {
        ret_data = iter->DoFilter(ret_data);
    }
    return ret_data;
}

quint32 FilterBank::GetFilterNum()
{
    return m_filter_bank.size();
}

void FilterBank::clear()
{
    m_filter_bank.clear();
}
