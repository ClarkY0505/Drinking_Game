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
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include <iostream>

TitleScreen::~TitleScreen() {
    if (_background) {
        SDL_DestroyTexture(_background);
    }

    if (_fog) {
        SDL_DestroyTexture(_fog);
    }
}

bool TitleScreen::init(SDL_Renderer* r) {
    _background = IMG_LoadTexture(r, "assets/images/background.png");

    if (!_background) {
        SDL_Log("Load background failed: %s", IMG_GetError());
        return false;
    }

    _fog = IMG_LoadTexture(r, "assets/images/fog.png");
    if (!_fog) {
        SDL_Log("load fog failed: %s", IMG_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(_fog, SDL_BLENDMODE_BLEND);
    // SDL_SetTextureAlphaMod(_fog, 60);

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
    // 更新雾位置
    _fogX_1 -= _fogSpeed_1 * dt;
    if (_fogX_1 <= -1280) {
        _fogX_1 = 0;
    }

    _fogX_2 -= _fogSpeed_2 * dt;
    if (_fogX_2 <= -1280) {
        _fogX_2 = 0;
    }

    _fogX_3 += _fogSpeed_3 * dt;
    if (_fogX_3 >= 1280) {
        _fogX_3 = 0;
    }
}

void TitleScreen::render_fog_layer(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                                   SDL_RendererFlip flip) {
    // 修改雾的薄厚
    SDL_SetTextureAlphaMod(_fog, alpha);

    SDL_Rect fog_1{static_cast<int>(x), y, w, h};
    SDL_Rect fog_2{static_cast<int>(x) + w, y, w, h};

    SDL_RenderCopyEx(r, _fog, nullptr, &fog_1, 0, nullptr, flip);
    SDL_RenderCopyEx(r, _fog, nullptr, &fog_2, 0, nullptr, flip);
}

void TitleScreen::render_fog_layer_right(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,SDL_RendererFlip flip)
{
    SDL_SetTextureAlphaMod(_fog, alpha);

    SDL_Rect fog_1{static_cast<int>(x), y, w, h};
    SDL_Rect fog_2{static_cast<int>(x) - w, y, w, h};

    SDL_RenderCopyEx(r, _fog, nullptr, &fog_1, 0, nullptr, flip);
    SDL_RenderCopyEx(r, _fog, nullptr, &fog_2, 0, nullptr, flip);
}

void TitleScreen::on_render(SDL_Renderer* r) {
    // SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
    // SDL_RenderClear(r);
    SDL_RenderCopy(r, _background, nullptr, nullptr);

    // 渲染两层雾，达到循环滚动
    float alpha_1 = 60 + 20 * sin(SDL_GetTicks() * 0.0003f);
    float alpha_2 = 50 + 25 * sin(SDL_GetTicks() * 0.00025f);
    float alpha_3 = 40 + 15 * sin(SDL_GetTicks() * 0.00035f);
    // SDL_SetTextureAlphaMod(_fog, static_cast<Uint8>(alpha));
    render_fog_layer_right(r, _fogX_3, 250, 1280, 400, static_cast<Uint8>(alpha_3), SDL_FLIP_HORIZONTAL);

    render_fog_layer(r, _fogX_1, 330, 1280, 360, static_cast<Uint8>(alpha_1), SDL_FLIP_NONE);

    render_fog_layer(r, _fogX_2, 430, 1280, 300, static_cast<Uint8>(alpha_2), SDL_FLIP_NONE);
}
