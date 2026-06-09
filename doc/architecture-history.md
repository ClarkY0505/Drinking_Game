# 架构历史更新

## 2026-06-04 轻量级引擎架构评审与转换建议

### 当前判断

当前项目已经具备轻量级 SDL 游戏引擎的基本雏形：

- `Game` 负责程序初始化、主循环、事件处理、更新和渲染。
- `Screen` 定义界面生命周期接口。
- `ScreenManager` 使用屏幕栈管理当前界面。
- `TitleScreen`、`GameScreen` 等具体界面承载游戏画面和交互逻辑。
- `Button` 作为简单 UI 控件处理点击区域判断。

整体结构足够小，抽象数量少，适合当前项目规模。`Screen` + `ScreenManager` 的方向是合理的，可以继续作为轻量级引擎的核心。

### 当前主要问题

#### 1. `Game` 职责偏重

`Game` 当前同时负责：

- SDL 初始化和关闭。
- window / renderer 创建与销毁。
- 主循环。
- 事件分发。
- 界面跳转。
- 游戏运行状态维护。

短期可以接受，但随着界面和游戏流程增加，`Game` 会变成过重的中央控制类。

#### 2. 界面跳转被写死在主循环中

当前 `Enter` 进入 `GameScreen`、`Esc` 返回或退出的逻辑写在 `Game::handle_events()` 中，并通过 `in_game` 判断当前状态。

这种方式在只有标题界面和游戏界面时简单有效，但当后续增加大厅、职业选择、设置、暂停菜单等界面后，会导致主循环持续膨胀。

#### 3. SDL 资源管理分散

`TitleScreen` 直接持有 `SDL_Texture*`，并在析构函数中手动 `SDL_DestroyTexture()`。

这种方式对小项目可用，但每个 screen 都重复写资源释放逻辑后，容易出现：

- init 中途失败时资源释放不完整。
- 重复销毁或忘记销毁。
- 加载和错误处理代码分散。

#### 4. `Screen` 生命周期需要保持职责清晰

当前 `Screen` 接口包含：

- `init()`
- `on_enter()`
- `on_exit()`
- `on_event()`
- `on_update()`
- `on_render()`

接口本身合理，但需要约定职责：

- `init()` 负责资源加载。
- `on_enter()` 负责进入界面时的状态初始化。
- `on_exit()` 负责离开界面时的状态处理。
- 资源释放优先交给 RAII 或析构函数。

### 建议转换方向

#### 阶段一：保留现有架构，先清理边界

不建议立刻做大规模重构。当前最适合的做法是保留：

- `Screen`
- `ScreenManager`
- `Game` 主循环
- 具体 screen 类

优先完成两个小转换：

1. 引入 screen 导航命令。
2. 引入 SDL 资源 RAII 包装。

#### 阶段二：把导航从 `Game` 移到 screen 决策

建议让 screen 通过命令告诉主循环下一步动作，而不是让 `Game` 硬编码所有跳转。

可选命令：

```cpp
enum class ScreenCommandType {
    None,
    Push,
    Pop,
    Quit
};
```

后续可以扩展为：

```cpp
struct ScreenCommand {
    ScreenCommandType type = ScreenCommandType::None;
    std::unique_ptr<Screen> screen;
};
```

这样 `TitleScreen` 可以在点击开始按钮后返回 `Push(GameScreen)`，`GameScreen` 可以在按下 `Esc` 后返回 `Pop`，`Game` 只负责执行命令。

转换后的职责关系：

- `Screen`：决定当前界面的交互结果。
- `Game`：执行主循环和 screen 命令。
- `ScreenManager`：维护 screen 栈。

#### 阶段三：为 SDL 资源增加 RAII

建议定义轻量资源类型：

```cpp
using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
```

然后 screen 中使用：

```cpp
TexturePtr _background{nullptr, SDL_DestroyTexture};
TexturePtr _fog{nullptr, SDL_DestroyTexture};
```

这样 texture 会自动释放，减少手动析构代码。

后续如果资源更多，再考虑做一个简单的 `TextureLoader` 或 `AssetManager`。目前不建议提前做完整资源系统。

#### 阶段四：按需要拆分 `Application`

当 `Game` 继续变重时，可以拆分为：

- `Application`：负责 SDL 生命周期、窗口、renderer、主循环。
- `Game`：负责游戏启动 screen、游戏流程和全局游戏状态。
- `ScreenManager`：负责 screen 栈。

这一步不急，等界面数量和游戏状态明显增多后再做更合适。

### 推荐优先级

1. 保持 `ScreenManager` 简洁，不扩大它的职责。
2. 先把硬编码导航从 `Game::handle_events()` 移出。
3. 给 SDL texture 增加 RAII 包装。
4. 再考虑是否拆分 `Application`。
5. 暂时不要引入复杂 ECS、事件总线、完整资产系统或场景图。

### 结论

当前设计符合轻量级引擎的早期形态。主要问题不是抽象不足，而是 `Game` 类开始承担过多职责。

下一步应该做小规模架构转换：让 screen 自己表达导航意图，让 `Game` 只执行主循环和命令，让 SDL 资源释放自动化。这样可以继续保持轻量，同时为后续增加大厅、职业选择、暂停菜单等界面留出清晰扩展空间。
