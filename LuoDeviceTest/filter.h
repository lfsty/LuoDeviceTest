#ifndef FILTER_H
#define FILTER_H
#include <QObject>
class Filter
{
public:
    Filter(int N, std::vector<double> A, std::vector<double> B);
    Filter(int N, const double* A, const double* B);
private:
    struct _parameter
    {
        std::vector<double> A;
        std::vector<double> B;
    };
private:
    _parameter m_filter;
    _parameter m_data;
    double mat_cal(std::vector<double> A, std::vector<double> B);
public:
    double DoFilter(double data);
};

#endif // FILTER_H
