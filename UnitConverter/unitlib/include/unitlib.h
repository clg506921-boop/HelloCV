#ifndef UNITLIB_H                 // 头文件保护：防止被重复包含导致重复定义
#define UNITLIB_H

// 换算函数：from/to 是单位名，value 是待换算数值
// 返回换算结果；单位无法识别时返回 NAN
double convert(const char* from, const char* to, double value);

// 把菜单编号翻成单位名：choice 1~6，which 1=源单位 2=目标单位；非法返回 0
const char* unit_name(int choice, int which);
#endif
