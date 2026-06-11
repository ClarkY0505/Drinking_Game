# 构建方法说明

本文档说明当前项目支持的构建方式、命令和用途。项目使用 CMake 3.22+、C++17、SDL2 和 SDL2_image。

## 1. 默认桌面构建

```bash
cmake -S . -B build
cmake --build build
```

作用：

- 配置并构建桌面版本。
- 默认会构建 `drinking_game` 可执行文件。
- 同时构建 `XunWoEngine` 动态库和静态库。
- 默认会启用 CTest，并构建测试目标。

主要产物：

```text
build/drinking_game
lib/libXunWoEngine.so
lib/libXunWoEngine.a
```

适用场景：

- 本地开发。
- 验证完整桌面程序是否能正常编译。
- 需要同时生成游戏可执行文件和引擎库文件。

## 2. 运行桌面程序

```bash
./build/drinking_game
```

作用：

- 运行默认桌面构建生成的游戏程序。

适用场景：

- 手动验证游戏流程、窗口创建、资源加载和 SDL 渲染行为。

## 3. 仅构建引擎库

```bash
cmake -S . -B build-lib-only -DDRINKING_GAME_BUILD_LIB_ONLY=ON
cmake --build build-lib-only
```

作用：

- 只构建 `XunWoEngine` 动态库和 `XunWoEngine_static` 静态库。
- 不构建 `drinking_game` 桌面可执行文件。
- 不构建 Android `main` 目标。
- 不构建测试可执行文件。
- 不查找 `SDL2_image`，因为当前 engine 源码只依赖 SDL2。

主要产物：

```text
lib/libXunWoEngine.so
lib/libXunWoEngine.a
```

适用场景：

- 只需要导出 engine 库给其他项目或平台使用。
- 快速验证 engine 层代码是否可单独编译。
- 避免构建游戏层、测试层和 SDL2_image 相关依赖。

## 4. 只构建指定库目标

动态库：

```bash
cmake --build build-lib-only --target XunWoEngine
```

静态库：

```bash
cmake --build build-lib-only --target XunWoEngine_static
```

作用：

- 在已经配置好的构建目录中，只构建指定的 engine 库目标。

适用场景：

- 只需要更新动态库或静态库其中一个产物。
- CI 或脚本中按目标精确构建。

## 5. 指定 SDL2 路径构建

```bash
cmake -S . -B build -DSDL2_DIR=/path/to/SDL2
cmake --build build
```

作用：

- 显式指定 SDL2 的位置。
- 当系统无法自动发现 SDL2，或项目使用 vendored SDL2 时使用。

`SDL2_DIR` 可以指向：

- 包含 `CMakeLists.txt` 的 SDL2 源码目录。
- 包含 `SDL2Config.cmake` 或 `sdl2-config.cmake` 的 CMake package 目录。

适用场景：

- SDL2 没有安装到系统默认搜索路径。
- 使用本地依赖目录或第三方依赖缓存。

## 6. 不构建桌面可执行文件

```bash
cmake -S . -B build-objects -DDRINKING_GAME_BUILD_DESKTOP=OFF
cmake --build build-objects
```

作用：

- 关闭桌面可执行文件构建。
- 非 Android 环境下会构建 `drinking_game` object library。
- engine 动态库和静态库仍会构建。

适用场景：

- 只想检查源码对象是否能编译。
- 当前 `src/core/main.cpp` 没有可用入口时，避免生成桌面可执行文件。

注意：

- 如果目标是只构建 engine 库，优先使用 `DRINKING_GAME_BUILD_LIB_ONLY=ON`。

## 7. 运行测试

```bash
ctest --test-dir build --output-on-failure
```

作用：

- 运行默认构建目录中的 CTest 测试。
- 当前测试目标为 `screen_command_compile_test`。

适用场景：

- 验证 public header 和基础 screen command API 能正常编译和运行。
- 在修改 CMake、engine header 或 screen 基础类型后做回归检查。

注意：

- 使用 `DRINKING_GAME_BUILD_LIB_ONLY=ON` 时不会生成测试可执行文件。
- 测试应在普通构建目录中运行，例如 `build/`。

## 8. Android Release 构建

```bash
cd android
gradle assembleRelease
```

作用：

- 构建 Android release 应用。
- Android 分支会使用 `main` 共享库目标作为入口库。

前置条件：

- 已安装 Android SDK。
- 已安装 Android NDK。
- 已安装 CMake 3.22.1。
- 项目期望存在 `../deps/SDL/` 依赖。

适用场景：

- 生成 Android release 包。
- 验证移动平台构建链路。

## 9. 产物目录

engine 库文件统一输出到项目根目录的 `lib/`：

```text
lib/libXunWoEngine.so
lib/libXunWoEngine.a
```

桌面可执行文件输出到构建目录：

```text
build/drinking_game
```

`build/`、object 文件、可执行文件、缓存和生成库文件都属于构建产物，不应提交到版本库。

## 10. 常用参数速查

| 参数 | 默认值 | 作用 |
| --- | --- | --- |
| `DRINKING_GAME_BUILD_DESKTOP` | `ON` | 控制非 Android 环境下是否构建桌面可执行文件 |
| `DRINKING_GAME_BUILD_LIB_ONLY` | `OFF` | 只构建 `XunWoEngine` 动态库和静态库 |
| `SDL2_DIR` | 空 | 指定 SDL2 源码目录或 CMake package 目录 |

推荐命令：

```bash
# 完整桌面构建
cmake -S . -B build
cmake --build build

# 仅构建 engine 库
cmake -S . -B build-lib-only -DDRINKING_GAME_BUILD_LIB_ONLY=ON
cmake --build build-lib-only

# 构建后运行测试
ctest --test-dir build --output-on-failure
```
