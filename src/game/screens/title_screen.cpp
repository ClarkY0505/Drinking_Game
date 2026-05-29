/*******************************************
 * Title screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#include "game/title_screen.h"

#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include <iostream>

TitleScreen::~TitleScreen() {
    if (_background) {
        SDL_DestroyTexture(_background);
    }
}

bool TitleScreen::init(SDL_Renderer* r) {
    _background = IMG_LoadTexture(r, "assets/images/background.png");

    if (!_background) {
        SDL_Log("Load background failed: %s", IMG_GetError());
        return false;
    }

    return true;
}

void TitleScreen::on_enter() {
    std::cout << "Enter Menu" << std::endl;
}

void TitleScreen::on_exit() {
    std::cout << "Exit Menu" << std::endl;
}

void TitleScreen::on_event(const SDL_Event& e) {
    if (SDL_KEYDOWN == e.type) {
        // switch(e.key.keysym.sym){
        //     case SDLK_RETURN:
        //         std::cout << "Start Game" << std::endl;
        //         break;
        //     case SDLK_ESCAPE:
        //         std::cout << "Exit Game" << std::endl;
        //         break;
        // }
        std::cout << "Menu Key Press" << std::endl;
    }
}

void TitleScreen::on_update(float dt) {
    (void)dt;
}

void TitleScreen::on_render(SDL_Renderer* r) {
    // SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
    // SDL_RenderClear(r);
    SDL_RenderCopy(r, _background, nullptr, nullptr);
}
