#ifndef FILTERBANK_H
#define FILTERBANK_H
#include "filter.h"

class FilterBank
{
public:
    FilterBank();
public:
    //添加滤波器
    void AddFilter(Filter filter);
    void AddFilter(int N, const double* DEN, const double* NUM);
    //滤波
    double DoFilter(double data);
    //滤波器个数
    quint32 GetFilterNum();
    //清空滤波器组
    void clear();
private:
    std::vector<Filter> m_filter_bank;
};

#endif // FILTERBANK_H
