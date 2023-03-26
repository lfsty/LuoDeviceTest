#ifndef FilterData250_H
#define FilterData250_H

namespace FilterData250
{
    namespace HP0_1
    {
        // 0.1Hz高通
        /*
         * 离散时间 IIR 滤波器(实数)
         * ----------------
         * 滤波器结构  : 直接 II 型
         * 分子 长度  : 3
         * 分母 长度  : 3
         * 稳定     : 是
         * 线性相位   : 否
         */
        const int NL = 3;
        const double NUM[3] =
        {
            0.9982244250252,    -1.99644885005,   0.9982244250252
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,   -1.996445697381,   0.9964520027193
        };
    }
    namespace Notch50
    {
        // 50Hz陷波
        /*
         * 离散时间 IIR 滤波器(实数)
         * ----------------
         * 滤波器结构  : 直接 II 型
         * 分子 长度  : 3
         * 分母 长度  : 3
         * 稳定     : 是
         * 线性相位   : 否
         */
        const int NL = 3;
        const double NUM[3] =
        {
            0.9875889380903,  -0.6104117261971,   0.9875889380903
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,  -0.6104117261971,   0.9751778761806
        };

    }
}
#endif

