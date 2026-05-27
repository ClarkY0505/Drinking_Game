#include <SDL2/SDL.h>  // 引入 SDL2 的核心 API，比如窗口、事件、渲染器

#include <iostream>  // 引入 std::cerr，用来输出错误信息
#include <memory>    // 引入 std::unique_ptr 和 std::make_unique

#include "app/game.h"
#include "engine/game_screen.h"   // 引入游戏界面 GameScreen
#include "engine/title_screen.h"  // 引入标题界面 TitleScreen
#include "game/screen_manager.h"  // 引入屏幕管理器 ScreenManager

int main() {  // 程序入口函数
    Game& game = Game::instance();
    if (!game.init()) {
        return -1;
    }

    game.run();
    game.shutdown();
    return 0;  // 返回 0，表示程序正常结束
}
