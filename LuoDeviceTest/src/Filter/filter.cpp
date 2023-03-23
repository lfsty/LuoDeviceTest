#include "filter.h"
double mat_cal(const _Queue<double>& A, const _Queue<double>& B)
{
    {
        if (A.size() != B.size())
        {
            throw std::runtime_error("size error");
        }
        double data = 0;
        for (int i = 0; i < A.size(); i++)
        {
            data += A[i] * B[i];
        }
        return data;
    }
}
