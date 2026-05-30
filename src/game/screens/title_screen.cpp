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

#include <cmath>
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
    _fogTime += dt;

    const float layer_1_speed = _fogSpeed_1 + 2.0f * std::sin(_fogTime * 0.37f);
    const float layer_2_speed = _fogSpeed_2 + 3.5f * std::sin(_fogTime * 0.29f + 1.8f);
    const float layer_3_speed = _fogSpeed_3 + 1.5f * std::sin(_fogTime * 0.21f + 3.4f);

    _fogX_1 -= layer_1_speed * dt;
    while (_fogX_1 <= -1280.0f) {
        _fogX_1 += 1280.0f;
    }

    _fogX_2 -= layer_2_speed * dt;
    while (_fogX_2 <= -1280.0f) {
        _fogX_2 += 1280.0f;
    }

    _fogX_3 += layer_3_speed * dt;
    while (_fogX_3 >= 1280.0f) {
        _fogX_3 -= 1280.0f;
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

void TitleScreen::render_fog_layer_right(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                                         SDL_RendererFlip flip) {
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

    const int y_1 = 330 + static_cast<int>(4.0f * std::sin(_fogTime * 0.41f + 0.7f));
    const int y_2 = 430 + static_cast<int>(6.0f * std::sin(_fogTime * 0.33f + 2.2f));
    const int y_3 = 250 + static_cast<int>(3.0f * std::sin(_fogTime * 0.27f + 4.1f));

    const float alpha_1 =
        // 58.0f + 14.0f * std::sin(_fogTime * 0.21f) + 6.0f * std::sin(_fogTime * 0.07f + 1.4f);
        58.0f + 14.0f * std::sin(_fogTime * 0.53f) + 6.0f * std::sin(_fogTime * 0.17f + 1.4f);
    const float alpha_2 =
        // 48.0f + 16.0f * std::sin(_fogTime * 0.18f + 2.1f) + 7.0f * std::sin(_fogTime * 0.08f);
        48.0f + 16.0f * std::sin(_fogTime * 0.47f + 2.1f) + 7.0f * std::sin(_fogTime * 0.19f);
    const float alpha_3 =
        // 38.0f + 11.0f * std::sin(_fogTime * 0.24f + 3.6f) + 5.0f * std::sin(_fogTime * 0.09f);
        38.0f + 11.0f * std::sin(_fogTime * 0.59f + 3.6f) + 5.0f * std::sin(_fogTime * 0.23f);
    // SDL_SetTextureAlphaMod(_fog, static_cast<Uint8>(alpha));
    render_fog_layer_right(r, _fogX_3, y_3, 1280, 400, static_cast<Uint8>(alpha_3),
                           SDL_FLIP_HORIZONTAL);

    render_fog_layer(r, _fogX_1, y_1, 1280, 360, static_cast<Uint8>(alpha_1), SDL_FLIP_NONE);

    render_fog_layer(r, _fogX_2, y_2, 1280, 300, static_cast<Uint8>(alpha_2), SDL_FLIP_NONE);
}
