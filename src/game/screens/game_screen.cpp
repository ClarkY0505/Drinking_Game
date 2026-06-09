/*******************************************
 * Game Main Screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#include "game/game_screen.h"

#include <iostream>

bool GameScreen::init(SDL_Renderer* r) {
    (void)r;
    return true;
}

void GameScreen::on_enter() {
    std::cout << "Enter Game" << std::endl;
}

void GameScreen::on_exit() {
    std::cout << "Exit Game" << std::endl;
}

ScreenCommand GameScreen::on_event(const SDL_Event& e) {
    if (SDL_KEYDOWN == e.type) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            return ScreenCommand::pop();
        }

        std::cout << "Game Key Press" << std::endl;
    }

    return ScreenCommand::none();
}

void GameScreen::on_update(float dt) {
    (void)dt;
}

void GameScreen::on_render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 50, 50, 240, 255);
    SDL_RenderClear(r);
}
