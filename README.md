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
├── LLMGateway/       # 建议名称，非硬性要求
│   └── ...
└── ros2_ws/          # 第三次任务：ROS 2 练习
    └── src/
        └── ros2_lab/
            ├── CMakeLists.txt
            ├── package.xml
            ├── launch/
            │   └── demo.launch.py
            └── src/
                ├── talker.cpp
                ├── listener.cpp
                ├── add_two_ints_server.cpp
                ├── add_two_ints_client.cpp
                └── tf_broadcaster.cpp
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
环境部署完毕：

在ubuntu中firefox中依次打开百度、github仓库和deepseek网址，确认Ubuntu 网络正常，能打开训练仓库及 DeepSeek 开放平台。

<img width="722" height="402" alt="image" src="https://github.com/user-attachments/assets/55e9fb1c-a55f-46f7-977f-2efa2cb36b92" />

确认Ubuntu 中的 Git 已经能够访问 Gitee 训练仓库：

<img width="568" height="49" alt="image" src="https://github.com/user-attachments/assets/6165b514-97fd-401d-9abc-7bd6b58051e8" />

接下来打通Ubuntu中API为布置agent做准备：
(此处因为日常使用的是Claude和dsh，采用更加熟悉且容易部署的dsh)
<img width="1100" height="250" alt="image" src="https://github.com/user-attachments/assets/197c59ed-4ed3-46c3-a478-dbbea81a5134" />
<img width="739" height="317" alt="image" src="https://github.com/user-attachments/assets/817f67d1-4add-4be5-a00e-3cbd6c15349c" />

## ROS 2 部分（第三次任务）

环境：Ubuntu 22.04.5 + **ROS 2 Humble**（`/opt/ros/humble`），功能包 `ros2_lab`。

### 构建

```bash
cd ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select ros2_lab
source install/setup.bash
```

### 运行

| 命令 | 作用 |
| --- | --- |
| `ros2 run ros2_lab talker` | 每秒发布 `hello N`；参数 `publish_rate` / `message_prefix` / `max_count` |
| `ros2 run ros2_lab talker --ros-args -p publish_rate:=2.0 -p max_count:=3` | 启动时传参：2 Hz、发满 3 条停止 |
| `ros2 run ros2_lab listener` | 订阅 `chatter` 并打印 |
| `ros2 run ros2_lab add_two_ints_server` | 服务端，服务名 `add_two_ints` |
| `ros2 run ros2_lab add_two_ints_client 3 4` | 客户端，返回 `结果: 3 + 4 = 7` |
| `ros2 run ros2_lab tf_broadcaster` | 以 10 Hz 广播 `base_link → laser` |
| `ros2 launch ros2_lab demo.launch.py` | 一次启动 talker / listener / tf_broadcaster，话题 remap 到 `/demo/chatter` |

### 验证 TF

```bash
ros2 run tf2_ros tf2_echo base_link laser   # 每 0.1 秒打印平移与旋转
ros2 run tf2_tools view_frames              # 生成 frames.gv / frames.pdf（TF 树）
rviz2                                       # Fixed Frame 设为 base_link，Add → TF
```

### 参数说明（热更新的边界）

- `message_prefix`：`on_timer()` 每条消息现读参数，`ros2 param set` **立刻生效**；
- `publish_rate`：定时器周期在构造时算好，运行中改参数**不会**改变频率，只能重启或改代码；
- `max_count`：`0` 表示不限，否则发满即停。

### 语雀笔记（Markdown 导出）

| 笔记 | 语雀链接 | Markdown 导出 |
| --- | --- | --- |
| 《ROS 2 工作空间、节点与话题通信》 | （待补） | [notes/04-ROS2-工作空间-节点与话题.md](notes/04-ROS2-工作空间-节点与话题.md) |
| 《ROS 2 服务、参数、TF 与 launch》 | （待补） | [notes/05-ROS2-服务-参数-TF-launch.md](notes/05-ROS2-服务-参数-TF-launch.md) |

图片在 `notes/images/` 下，与 Markdown 里的相对路径一一对应。
