#include <iostream>
#include <limits>          // numeric_limits
#include <cstdio>          // printf
#include <cmath>           // isnan
#include "unitlib.h"       // 只需声明就能编译；实现等链接时从库里找

static void show_menu() {
    std::cout << "\n========== 单位转换器 ==========\n";
    std::cout << "  1. 摄氏度 (C)  -> 华氏度 (F)\n";
    std::cout << "  2. 华氏度 (F)  -> 摄氏度 (C)\n";
    std::cout << "  3. 米 (m)      -> 英尺 (ft)\n";
    std::cout << "  4. 英尺 (ft)   -> 米 (m)\n";
    std::cout << "  5. 磅 (lb)     -> 千克 (kg)\n";
    std::cout << "  6. 千克 (kg)   -> 磅 (lb)\n";
    std::cout << "  0. 退出\n";
    std::cout << "请选择 [0-6]: ";
}

// 清空输入缓冲；返回 false 表示输入已结束(EOF)
// 没有 EOF 检测的话，输入一旦结束就会无限空转
static bool clear_input() {
    if (std::cin.eof()) return false;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

int main() {
    for (;;) {                                     // 主循环：选 0 才退出
        show_menu();

        int choice;
        if (!(std::cin >> choice)) {               // 菜单处输入字母
            if (!clear_input()) break;             // EOF：干净退出
            std::cout << "输入无效：请输入 0-6 的数字。\n";
            continue;
        }
        if (choice == 0) { std::cout << "已退出。\n"; break; }
        if (choice < 1 || choice > 6) {
            std::cout << "编号超出范围：请输入 0-6。\n";
            continue;
        }

        std::cout << "请输入要换算的数值: ";
        double value;
        if (!(std::cin >> value)) {                // 数值处输入字母
            if (!clear_input()) break;
            std::cout << "数值无效：请输入数字，例如 1 或 -40.5。\n";
            continue;
        }

        const char* from = unit_name(choice, 1);
        const char* to   = unit_name(choice, 2);
        double r = convert(from, to, value);

        if (std::isnan(r)) std::cout << "换算失败：单位无法识别。\n";
        else std::printf("%.4f %s = %.4f %s\n", value, from, r, to);
    }
    return 0;
}
