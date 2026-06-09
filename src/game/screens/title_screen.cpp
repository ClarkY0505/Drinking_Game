/*******************************************
 * Title screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#include "game/title_screen.h"

#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include <cmath>
#include <iostream>

#include "game/game_screen.h"
#include "widgets/button.h"

bool TitleScreen::init(SDL_Renderer* r) {
    _background = make_texture_ptr(IMG_LoadTexture(r, "assets/images/background.png"));
    _buttons.push_back({Button{{1000, 50, 200, 60}}, []() {
                            SDL_Log("Start Button Clicked");
                            return ScreenCommand::push(std::make_unique<GameScreen>());
                        }});

    _buttons.push_back({Button{{1000, 130, 200, 60}}, []() {
                            SDL_Log("Menu Button Clicked");
                            return ScreenCommand::none();
                        }});

    _buttons.push_back({Button{{1000, 210, 200, 60}}, []() {
                            SDL_Log("Menu Button Clicked");
                            return ScreenCommand::none();
                        }});

    if (!_background) {
        SDL_Log("Load background failed: %s", IMG_GetError());
        return false;
    }

    _fog = make_texture_ptr(IMG_LoadTexture(r, "assets/images/fog.png"));
    if (!_fog) {
        SDL_Log("load fog failed: %s", IMG_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(_fog.get(), SDL_BLENDMODE_BLEND);

    return true;
}

void TitleScreen::on_enter() {
    std::cout << "Enter Menu" << std::endl;
}

void TitleScreen::on_exit() {
    std::cout << "Exit Menu" << std::endl;
}

ScreenCommand TitleScreen::on_event(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_RETURN:
                return ScreenCommand::push(std::make_unique<GameScreen>());
            case SDLK_ESCAPE:
                return ScreenCommand::quit();
            default:
                break;
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x;
        int y = e.button.y;

        for (auto& it : _buttons) {
            if (it.button.is_clicked(x, y)) {
                return it.on_click();
            }
        }
    }

    return ScreenCommand::none();
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
    SDL_SetTextureAlphaMod(_fog.get(), alpha);

    SDL_Rect fog_1{static_cast<int>(x), y, w, h};
    SDL_Rect fog_2{static_cast<int>(x) + w, y, w, h};

    SDL_RenderCopyEx(r, _fog.get(), nullptr, &fog_1, 0, nullptr, flip);
    SDL_RenderCopyEx(r, _fog.get(), nullptr, &fog_2, 0, nullptr, flip);
}

void TitleScreen::render_fog_layer_right(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                                         SDL_RendererFlip flip) {
    SDL_SetTextureAlphaMod(_fog.get(), alpha);

    SDL_Rect fog_1{static_cast<int>(x), y, w, h};
    SDL_Rect fog_2{static_cast<int>(x) - w, y, w, h};

    SDL_RenderCopyEx(r, _fog.get(), nullptr, &fog_1, 0, nullptr, flip);
    SDL_RenderCopyEx(r, _fog.get(), nullptr, &fog_2, 0, nullptr, flip);
}

void TitleScreen::on_render(SDL_Renderer* r) {
    SDL_RenderCopy(r, _background.get(), nullptr, nullptr);

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
    render_fog_layer_right(r, _fogX_3, y_3, 1280, 400, static_cast<Uint8>(alpha_3),
                           SDL_FLIP_HORIZONTAL);

    render_fog_layer(r, _fogX_1, y_1, 1280, 360, static_cast<Uint8>(alpha_1), SDL_FLIP_NONE);

    render_fog_layer(r, _fogX_2, y_2, 1280, 300, static_cast<Uint8>(alpha_2), SDL_FLIP_NONE);

    for (auto& it : _buttons) {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderFillRect(r, &it.button.rect);
    }
}
