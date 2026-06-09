/*******************************************
 * Screen Base Class
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#ifndef DRINKING_GAME_UI_SCREEN_H
#define DRINKING_GAME_UI_SCREEN_H

#include <SDL2/SDL.h>

#include <memory>

class Screen;

enum class ScreenCommandType { None, Push, Pop, Quit };

struct ScreenCommand {
    ScreenCommandType type = ScreenCommandType::None;
    std::unique_ptr<Screen> screen;

    static ScreenCommand none() {
        return {};
    }

    static ScreenCommand push(std::unique_ptr<Screen> next_screen) {
        ScreenCommand command;
        command.type = ScreenCommandType::Push;
        command.screen = std::move(next_screen);
        return command;
    }

    static ScreenCommand pop() {
        ScreenCommand command;
        command.type = ScreenCommandType::Pop;
        return command;
    }

    static ScreenCommand quit() {
        ScreenCommand command;
        command.type = ScreenCommandType::Quit;
        return command;
    }
};

class Screen {
   public:
    virtual ~Screen() = default;
    virtual void on_enter() = 0;
    virtual void on_exit() = 0;
    virtual ScreenCommand on_event(const SDL_Event& e) = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_render(SDL_Renderer* r) = 0;
    virtual bool init(SDL_Renderer* r) = 0;
};

#endif
