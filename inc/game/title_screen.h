/*******************************************
 * Title screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#ifndef DRINKING_GAME_UI_TITLE_SCREEN_H
#define DRINKING_GAME_UI_TITLE_SCREEN_H

#include "engine/screen.h"

class TitleScreen : public Screen {
   public:
    ~TitleScreen();

    void on_enter() override;
    void on_exit() override;
    void on_event(const SDL_Event& e) override;
    void on_update(float dt) override;
    void on_render(SDL_Renderer* r) override;

    bool init(SDL_Renderer* r) override;

   private:
    void render_fog_layer(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                          SDL_RendererFlip flip);
    void render_fog_layer_right(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                                SDL_RendererFlip flip);

   private:
    SDL_Texture* _background = nullptr;
    SDL_Texture* _fog = nullptr;

    float _fogX_1 = 0.0f;
    float _fogX_2 = 0.0f;
    float _fogX_3 = 0.0f;
    float _fogTime = 0.0f;
    float _fogSpeed_1 = 15.0f;
    float _fogSpeed_2 = 30.0f;
    float _fogSpeed_3 = 8.0f;
};

#endif
