#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class _Queue
{
public:
    _Queue<T>();
    _Queue<T>(int length);
    _Queue<T>(const _Queue& copy);
    ~_Queue();
public:
    T operator[](int i);
    T operator[](int i) const;
    void add_data(T data);
    void init(std::vector<T> data);
    void init(const double* data);
    void SetQueueSize(int size);
    int size()
    {
        return m_length;
    }
    int size() const
    {
        return m_length;
    }
private:
    T* m_data = nullptr;
private:
    int m_front = 0;
    int m_length = 0;
};
double mat_cal(const _Queue<double>& A, const _Queue<double>& B);
template<typename value_type>
class Filter
{
public:
    Filter(int Order, const std::vector<double>& B, const std::vector<double>& A);
    Filter(int Order, const double* B, const double* A);
private:
    struct _parameter
    {
        _Queue<double> A;
        _Queue<double> B;
    };
private:
    _parameter m_filter;
    _parameter m_data;
    int m_order = 0;
public:
    double DoFilter(value_type data);
    std::vector<double> DoFilter(const value_type* data, int size);
    std::vector<double> DoFilter(std::vector<value_type> data);
};


template<typename T>
inline _Queue<T>::_Queue()
{
}

template<typename T>
inline _Queue<T>::_Queue(int length)
    : m_length(length),
      m_front(0)
{
    m_data = new T[length];
    memset(m_data, 0, length * sizeof(T));
}

template<typename T>
inline _Queue<T>::_Queue(const _Queue& copy)
{
    m_length = copy.m_length;
    m_front = 0;

    m_data = new T[m_length];
    memcpy(m_data, copy.m_data, m_length * sizeof(T));
    m_front = copy.m_front;
    m_front = copy.m_front;
}

template<typename T>
inline _Queue<T>::~_Queue()
{
    if (m_data != nullptr)
    {
        delete[] m_data;
    }
}

template<typename T>
inline T _Queue<T>::operator[] (int i)
{
    if (m_data == nullptr)
    {
        throw std::runtime_error("not set queue size");
    }
    if (i >= m_length)
    {
        throw std::out_of_range("index out of range");
    }
    else
    {
        return m_data[(m_front + i) % m_length];
    }
}

template<typename T>
inline T _Queue<T>::operator[](int i) const
{
    if (m_data == nullptr)
    {
        throw std::runtime_error("not set queue size");
    }
    if (i >= m_length)
    {
        throw std::out_of_range("index out of range");
    }
    else
    {
        return m_data[(m_front + i) % m_length];
    }
}


template<typename T>
inline void _Queue<T>::add_data(T data)
{
    if (m_data == nullptr)
    {
        throw std::runtime_error("not set queue size");
    }
    m_front--;
    if (m_front < 0)
    {
        m_front = m_length - 1;
    }
    m_data[m_front] = data;
}

template<typename T>
inline void _Queue<T>::init(std::vector<T> data)
{
    if (m_data == nullptr)
    {
        throw std::runtime_error("not set queue size");
    }
    if (m_length != data.size())
    {
        throw std::runtime_error("size not fit");
    }
    for (int i = 0; i < m_length; i++)
    {
        m_data[i] = data[i];
    }
}

template<typename T>
inline void _Queue<T>::init(const double* data)
{
    if (m_data == nullptr)
    {
        throw std::runtime_error("not set queue size");
    }
    for (int i = 0; i < m_length; i++)
    {
        m_data[i] = data[i];
    }
}

template<typename T>
inline void _Queue<T>::SetQueueSize(int size)
{
    if (m_data != nullptr)
    {
        throw std::runtime_error("queue size already set");
    }
    m_length = size;
    m_front = 0;

    m_data = new T[size];
    memset(m_data, 0, size * sizeof(T));
}

template<typename value_type>
inline Filter<value_type>::Filter(int Order, const std::vector<double>& B, const std::vector<double>& A)
{
    m_filter.A.SetQueueSize(Order);
    m_filter.A.init(std::vector<double>(A.begin() + 1, A.end()));
    m_filter.B.SetQueueSize(Order + 1);
    m_filter.B.init(B);
    m_data.A.SetQueueSize(Order + 1);
    m_data.B.SetQueueSize(Order);
    m_order = Order;
}

template<typename value_type>
inline Filter<value_type>::Filter(int Order, const double* B, const double* A)
{
    m_filter.A.SetQueueSize(Order);
    m_filter.A.init(A + 1);
    m_filter.B.SetQueueSize(Order + 1);
    m_filter.B.init(B);
    m_data.A.SetQueueSize(Order + 1);
    m_data.B.SetQueueSize(Order);
    m_order = Order;
}

template<typename value_type>
inline double Filter<value_type>::DoFilter(value_type data)
{
    m_data.A.add_data(data);
    // 差分方程
    double filter_data = mat_cal(m_filter.B, m_data.A) - mat_cal(m_filter.A, m_data.B);
    m_data.B.add_data(filter_data);

    return filter_data;
}

template<typename value_type>
inline std::vector<double> Filter<value_type>::DoFilter(const value_type* data, int size)
{
    std::vector<value_type> ret_data(data, data + size);
    return DoFilter(ret_data);
}

template<typename value_type>
inline std::vector<double> Filter<value_type>::DoFilter(std::vector<value_type> data)
{
    std::vector<double> ret_data(data.begin(), data.end());

    for (int i = 0; i < data.size(); i++)
    {
        ret_data[i] = 0;
        for (int j = 0; j < m_order + 1; j++)
        {
            if (i >= j)
            {
                ret_data[i] = ret_data[i] + m_filter.B[j] * data[i - j];
            }
        }
        for (int l = 0; l < m_order; l++)
        {
            if (i > l)
            {
                ret_data[i] = ret_data[i] - m_filter.A[l] * ret_data[i - l - 1];
            }
        }
    }

    return ret_data;
}

#endif // FILTER_H
