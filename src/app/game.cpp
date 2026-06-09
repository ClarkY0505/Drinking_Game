#include "app/game.h"

#include <SDL2/SDL_image.h>

#include <iostream>

#include "game/title_screen.h"

namespace {  // 匿名命名空间：这里的变量和函数只在本 cpp 文件中可见
constexpr int WINDOW_WIDTH = 1280;  // 窗口宽度，单位是像素
constexpr int WINDOW_HEIGHT = 720;  // 窗口高度，单位是像素

float seconds_since(Uint64 previous, Uint64 current) {  // 计算两次计时器之间经过了多少秒
    return static_cast<float>(current - previous) /  // 当前计数减去上一次计数，得到经过的计数差
           static_cast<float>(SDL_GetPerformanceFrequency());  // 除以每秒计数次数，把计数差转换成秒
}
}  // namespace

Game::Game() = default;

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError()
                  << std::endl;  // 初始化失败时打印 SDL 的错误信息
        return false;
    }

    // 支持PNG加载
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("IMG_Init Error: %s", IMG_GetError());
        SDL_Quit();
        return false;
    }

    _window = SDL_CreateWindow("Drinking Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (nullptr == _window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError()
                  << std::endl;  // 打印窗口创建失败的原因
        IMG_Quit();
        SDL_Quit();  // 释放 SDL_Init 初始化过的 SDL 资源
        return false;
    }

    _renderer =                                        // 保存 SDL 创建出来的渲染器指针
        SDL_CreateRenderer(_window,                    // 渲染器绑定到刚创建的窗口
                           -1,                         // 使用默认的渲染驱动
                           SDL_RENDERER_ACCELERATED |  // 尽量使用硬件加速渲染
                               SDL_RENDERER_PRESENTVSYNC);  // 开启垂直同步，减少画面撕裂

    if (_renderer == nullptr) {  // 如果渲染器创建失败，SDL 会返回空指针
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError()
                  << std::endl;      // 打印渲染器创建失败的原因
        SDL_DestroyWindow(_window);  // 已经创建了窗口，所以先销毁窗口
        _window = nullptr;
        IMG_Quit();
        SDL_Quit();    // 再释放 SDL 全局资源
        return false;  // 返回非 0，表示程序异常退出
    }

    if (!push_screen(std::make_unique<TitleScreen>())) {  // 把标题界面作为第一个显示的界面
        shutdown();
        return false;
    }

    return true;
}

void Game::run() {
    previous_counter = SDL_GetPerformanceCounter();  // 记录上一帧的高精度计时器数值

    while (_running && !_screen_manager.empty()) {
        handle_events();

        float dt = calc_delta_time();
        update(dt);

        render();
    }
}

void Game::handle_events() {
    SDL_Event event;  // SDL 事件对象，用来接收键盘、鼠标、关闭窗口等事件
    while (SDL_PollEvent(&event) != 0) {  // 不断取出事件队列中的事件；没有事件时返回 0
        if (event.type == SDL_QUIT) {  // 用户点击窗口关闭按钮时会收到 SDL_QUIT
            _running = false;          // 通知主循环结束
            continue;                  // 这个事件已经处理完，继续处理下一个事件
        }

        Screen* current_screen =
            _screen_manager.current();  // 取出当前栈顶界面，也就是正在显示的界面
        if (current_screen != nullptr) {  // 防止屏幕栈为空时访问空指针
            execute_command(current_screen->on_event(event));
        }
    }
}

float Game::calc_delta_time() {
    const Uint64 current_counter = SDL_GetPerformanceCounter();  // 获取当前帧的高精度计时器数值
    const float dt =
        seconds_since(previous_counter, current_counter);  // 计算上一帧到这一帧经过了多少秒
    previous_counter = current_counter;  // 更新上一帧计时器，供下一帧计算 dt

    return dt;
}

void Game::update(float dt) {
    if (Screen* screen = _screen_manager.current()) {
        screen->on_update(dt);
    }
}

void Game::render() {
    Screen* current_screen =
        _screen_manager.current();  // 再次取当前界面，因为事件处理可能改变了界面
    if (current_screen != nullptr) {           // 如果当前还有界面
        current_screen->on_render(_renderer);  // 调用当前界面的绘制逻辑，画到 renderer 上
        SDL_RenderPresent(_renderer);  // 把 renderer 中画好的内容真正显示到窗口上
    }
}

bool Game::push_screen(std::unique_ptr<Screen> screen) {
    if (!screen->init(_renderer)) {
        return false;
    }

    _screen_manager.push(std::move(screen));
    return true;
}

void Game::execute_command(ScreenCommand command) {
    switch (command.type) {
        case ScreenCommandType::None:
            break;
        case ScreenCommandType::Push:
            if (!command.screen || !push_screen(std::move(command.screen))) {
                _running = false;
            }
            break;
        case ScreenCommandType::Pop:
            _screen_manager.pop();
            break;
        case ScreenCommandType::Quit:
            _running = false;
            break;
    }
}

void Game::shutdown() {
    _screen_manager.clear();

    SDL_DestroyRenderer(_renderer);  // 销毁渲染器，释放渲染相关资源
    SDL_DestroyWindow(_window);      // 销毁窗口，释放窗口相关资源
    IMG_Quit();
    SDL_Quit();  // 关闭 SDL，释放 SDL 全局资源
}
