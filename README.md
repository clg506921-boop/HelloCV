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
