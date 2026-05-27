#ifndef DRINKING_GAME_APP_GAME_H
#define DRINKING_GAME_APP_GAME_H

#include "game/screen_manager.h"

class Game {
   public:
    static Game& instance() {
        static Game instance;
        return instance;
    }

    bool init();
    void run();
    void shutdown();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

   private:
    Game();
    ~Game() = default;

    void handle_events();
    void update(float dt);
    void render();
    float calc_delta_time();

   private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;

    ScreenManager _screen_manager;

    bool _running = true;
    bool in_game = false;

    Uint64 previous_counter = 0;
};

#endif
