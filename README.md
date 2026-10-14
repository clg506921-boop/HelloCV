## 仓库组织

```text
HelloCV/
├── README.md
├── Notes.md          # 三篇语雀笔记链接
├── .gitignore
├── UnitConverter/
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp
├── unitlib/
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── unitlib.h
│   └── src/
│       └── unitlib.cpp
└── LLMGateway/       # 建议名称，非硬性要求
    └── ...
```

## 检查开发环境

在 Ubuntu 终端中执行：
```
g++ --version
cmake --version
git --version
python3 --version
```
结果
```
g++ (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake version 3.22.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).
git version 2.34.1
Python 3.10.12
```
环境部署完毕

在ubuntu中firefox中依次打开百度、github仓库和deepseek网址，确认Ubuntu 网络正常，能打开训练仓库及 DeepSeek 开放平台。

<img width="722" height="402" alt="image" src="https://github.com/user-attachments/assets/55e9fb1c-a55f-46f7-977f-2efa2cb36b92" />

确认Ubuntu 中的 Git 已经能够访问 Gitee 训练仓库

<img width="568" height="49" alt="image" src="https://github.com/user-attachments/assets/6165b514-97fd-401d-9abc-7bd6b58051e8" />


