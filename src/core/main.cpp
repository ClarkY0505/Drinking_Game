#include <SDL2/SDL.h>  // 引入 SDL2 的核心 API，比如窗口、事件、渲染器

#include <iostream>  // 引入 std::cerr，用来输出错误信息
#include <memory>    // 引入 std::unique_ptr 和 std::make_unique

#include "engine/game_screen.h"  // 引入游戏界面 GameScreen
#include "engine/title_screen.h" // 引入标题界面 TitleScreen
#include "game/screen_manager.h" // 引入屏幕管理器 ScreenManager

namespace {                         // 匿名命名空间：这里的变量和函数只在本 cpp 文件中可见
constexpr int WINDOW_WIDTH = 1280;  // 窗口宽度，单位是像素
constexpr int WINDOW_HEIGHT = 720;  // 窗口高度，单位是像素

float seconds_since(Uint64 previous, Uint64 current) {        // 计算两次计时器之间经过了多少秒
    return static_cast<float>(current - previous) /           // 当前计数减去上一次计数，得到经过的计数差
           static_cast<float>(SDL_GetPerformanceFrequency()); // 除以每秒计数次数，把计数差转换成秒
}
}  // namespace  // 匿名命名空间结束

int main() {                                                  // 程序入口函数
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {    // 初始化 SDL 的视频系统和事件系统；返回 0 表示成功
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl; // 初始化失败时打印 SDL 的错误信息
        return 1;                                             // 返回非 0，表示程序异常退出
    }

    SDL_Window* window =                                      // 保存 SDL 创建出来的窗口指针
        SDL_CreateWindow("Drinking Game",                     // 窗口标题
                         SDL_WINDOWPOS_CENTERED,              // 窗口 X 坐标居中
                         SDL_WINDOWPOS_CENTERED,              // 窗口 Y 坐标居中
                         WINDOW_WIDTH,                        // 窗口宽度
                         WINDOW_HEIGHT,                       // 窗口高度
                         SDL_WINDOW_SHOWN);                   // 创建后直接显示窗口

    if (window == nullptr) {                                  // 如果窗口创建失败，SDL 会返回空指针
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl; // 打印窗口创建失败的原因
        SDL_Quit();                                           // 释放 SDL_Init 初始化过的 SDL 资源
        return 1;                                             // 返回非 0，表示程序异常退出
    }

    SDL_Renderer* renderer =                                  // 保存 SDL 创建出来的渲染器指针
        SDL_CreateRenderer(window,                            // 渲染器绑定到刚创建的窗口
                           -1,                                // 使用默认的渲染驱动
                           SDL_RENDERER_ACCELERATED |         // 尽量使用硬件加速渲染
                               SDL_RENDERER_PRESENTVSYNC);    // 开启垂直同步，减少画面撕裂

    if (renderer == nullptr) {                                // 如果渲染器创建失败，SDL 会返回空指针
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl; // 打印渲染器创建失败的原因
        SDL_DestroyWindow(window);                            // 已经创建了窗口，所以先销毁窗口
        SDL_Quit();                                           // 再释放 SDL 全局资源
        return 1;                                             // 返回非 0，表示程序异常退出
    }

    ScreenManager screen_manager;                             // 创建屏幕管理器，用栈管理当前界面
    screen_manager.push(std::make_unique<TitleScreen>());     // 把标题界面压入管理器，作为第一个显示的界面

    bool running = true;                                      // 主循环开关；为 false 时程序准备退出
    bool in_game = false;                                     // 记录当前是否在游戏界面
    Uint64 previous_counter = SDL_GetPerformanceCounter();    // 记录上一帧的高精度计时器数值

    while (running && !screen_manager.empty()) {              // 只要程序还在运行，并且还有界面，就持续循环
        SDL_Event event;                                      // SDL 事件对象，用来接收键盘、鼠标、关闭窗口等事件
        while (SDL_PollEvent(&event) != 0) {                  // 不断取出事件队列中的事件；没有事件时返回 0
            bool handled_navigation = false;                  // 标记这个事件是否已经被 main 用来做界面切换

            if (event.type == SDL_QUIT) {                     // 用户点击窗口关闭按钮时会收到 SDL_QUIT
                running = false;                              // 通知主循环结束
                continue;                                     // 这个事件已经处理完，继续处理下一个事件
            }

            if (event.type == SDL_KEYDOWN) {                  // 如果当前事件是键盘按下
                switch (event.key.keysym.sym) {               // 根据具体按下的按键做不同处理
                    case SDLK_RETURN:                         // SDLK_RETURN 表示回车键
                        if (!in_game) {                       // 只有当前不在游戏界面时，回车才进入游戏
                            screen_manager.push(std::make_unique<GameScreen>()); // 创建游戏界面并压入管理器
                            in_game = true;                   // 标记当前已经进入游戏界面
                            handled_navigation = true;        // 标记这个按键已经用于界面切换
                        }
                        break;                                // 结束回车键的处理
                    case SDLK_ESCAPE:                         // SDLK_ESCAPE 表示 Esc 键
                        if (in_game) {                        // 如果当前在游戏界面，Esc 表示返回标题界面
                            screen_manager.pop();             // 弹出当前 GameScreen，并触发它的 on_exit()
                            in_game = false;                  // 标记当前回到非游戏界面
                            handled_navigation = true;        // 标记这个按键已经用于界面切换
                        } else {
                            running = false;                  // 如果当前在标题界面，Esc 表示退出程序
                            handled_navigation = true;        // 标记这个按键已经被 main 处理
                        }
                        break;                                // 结束 Esc 键的处理
                    default:                                  // 其他按键不在 main 中做界面切换
                        break;                                // 其他按键交给当前界面的 on_event 处理
                }
            }

            if (handled_navigation) {                         // 如果事件已经用于切换界面或退出
                continue;                                     // 不再把这个事件传给当前界面，避免重复处理
            }

            Screen* current_screen = screen_manager.current(); // 取出当前栈顶界面，也就是正在显示的界面
            if (current_screen != nullptr) {                  // 防止屏幕栈为空时访问空指针
                current_screen->on_event(event);              // 把事件交给当前界面自己处理
            }
        }

        const Uint64 current_counter = SDL_GetPerformanceCounter(); // 获取当前帧的高精度计时器数值
        const float dt = seconds_since(previous_counter, current_counter); // 计算上一帧到这一帧经过了多少秒
        previous_counter = current_counter;                    // 更新上一帧计时器，供下一帧计算 dt

        Screen* current_screen = screen_manager.current();     // 再次取当前界面，因为事件处理可能改变了界面
        if (current_screen != nullptr) {                       // 如果当前还有界面
            current_screen->on_update(dt);                     // 调用当前界面的更新逻辑，比如动画、状态变化
            current_screen->on_render(renderer);               // 调用当前界面的绘制逻辑，画到 renderer 上
            SDL_RenderPresent(renderer);                       // 把 renderer 中画好的内容真正显示到窗口上
        }
    }

    while (!screen_manager.empty()) {                          // 程序结束前，清空所有还在管理器里的界面
        screen_manager.pop();                                  // 每弹出一个界面都会调用它的 on_exit()
    }

    SDL_DestroyRenderer(renderer);                             // 销毁渲染器，释放渲染相关资源
    SDL_DestroyWindow(window);                                 // 销毁窗口，释放窗口相关资源
    SDL_Quit();                                                // 关闭 SDL，释放 SDL 全局资源

    return 0;                                                  // 返回 0，表示程序正常结束
}
