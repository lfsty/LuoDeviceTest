#include "filter.h"

#include "filter.h"
#include <stdexcept>

Filter::Filter(int N, std::vector<double> A, std::vector<double> B)
{
    if (A.size() != B.size() || N + 1 != A.size())
    {
        throw std::runtime_error("滤波器参数出错");
    }
    m_filter.A = A;
    m_filter.B = B;
    m_data.A.resize(N + 1);
    m_data.B.resize(N);
}

Filter::Filter(int N, const  double* A, const double* B)
{
    for(int i = 0; i < N + 1; i++)
    {
        m_filter.A.push_back(A[i]);
        m_filter.B.push_back(B[i]);
    }
    m_data.A.resize(N + 1);
    m_data.B.resize(N);
}

double Filter::mat_cal(std::vector<double> A, std::vector<double> B)
{
    if (A.size() != B.size())
    {
        throw std::runtime_error("矩阵计算大小出错");
    }
    double data = 0;
    for (int i = 0; i < A.size(); i++)
    {
        data += A[i] * B[i];
    }
    return data;
}

double Filter::DoFilter(double data)
{
    m_data.A.erase(m_data.A.end() - 1);
    m_data.A.insert(m_data.A.begin(), data);
    // 差分方程
    double filter_data = mat_cal(m_filter.B, m_data.A) - mat_cal(std::vector<double>(m_filter.A.begin() + 1, m_filter.A.end()), m_data.B);
    m_data.B.erase(m_data.B.end() - 1);
    m_data.B.insert(m_data.B.begin(), filter_data);

    return filter_data;
}
