#include "unitlib.h"       // 包含自己的声明：实现与声明不一致时编译期立刻报错
#include <cmath>           // NAN
#include <cstring>         // strcmp

double convert(const char* from, const char* to, double value) {

    // 摄氏 -> 华氏：F = C * 9/5 + 32  （先乘后加）
    if (std::strcmp(from, "C") == 0 && std::strcmp(to, "F") == 0)
        return value * 9.0 / 5.0 + 32.0;
    // 华氏 -> 摄氏：C = (F - 32) * 5/9 （必须先减 32）
    if (std::strcmp(from, "F") == 0 && std::strcmp(to, "C") == 0)
        return (value - 32.0) * 5.0 / 9.0;

    // 长度基准 = 米；重量基准 = 千克
    double base;
    if (std::strcmp(from, "m") == 0)       base = value;               // 米就是基准
    else if (std::strcmp(from, "ft") == 0) base = value * 0.3048;      // 1 ft = 0.3048 m
    else if (std::strcmp(from, "kg") == 0) base = value;               // 千克就是基准
    else if (std::strcmp(from, "lb") == 0) base = value * 0.45359237;  // 1 lb = 0.45359237 kg
    else return NAN;                                                   // 源单位不认识

    // 第二趟：基准 -> 目标（上面操作的逆运算，所以用除）
    if (std::strcmp(to, "m") == 0)       return base;
    else if (std::strcmp(to, "ft") == 0) return base / 0.3048;
    else if (std::strcmp(to, "kg") == 0) return base;
    else if (std::strcmp(to, "lb") == 0) return base / 0.45359237;
    else return NAN;                                                   // 目标单位不认识
}

const char* unit_name(int choice, int which) {
    //              choice:  1     2     3    4     5     6
    static const char* src[7] = {0, "C",  "F",  "m", "ft", "lb", "kg"};  // 源单位
    static const char* dst[7] = {0, "F",  "C", "ft",  "m", "kg", "lb"};  // 目标单位
    if (choice < 1 || choice > 6) return 0;        // 编号非法
    return which == 1 ? src[choice] : dst[choice];
}
