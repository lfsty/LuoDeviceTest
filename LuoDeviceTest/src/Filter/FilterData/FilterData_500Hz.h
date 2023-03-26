#ifndef FilterData500_H
#define FilterData500_H
namespace FilterData500
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
            0.9991118180796,   -1.998223636159,   0.9991118180796
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,   -1.998222847292,   0.9982244250264
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
            0.9937559649537,   -1.607962667624,   0.9937559649537
        };
        const int DL = 3;
        const double DEN[3] =
        {
            1,   -1.607962667624,   0.9875119299073
        };

    }
}


#endif
