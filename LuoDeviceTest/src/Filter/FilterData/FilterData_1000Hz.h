#ifndef FilterData1000_H
#define FilterData1000_H

namespace FilterData1000
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
            0.9995558103876,   -1.999111620775,   0.9995558103876
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,   -1.999111423471,   0.9991118180796
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
            0.9968682357708,   -1.896165420229,   0.9968682357708
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,   -1.896165420229,   0.9937364715416
        };

    }
}


#endif
