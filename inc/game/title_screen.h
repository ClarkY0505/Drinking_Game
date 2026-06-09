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

#include <functional>
#include <vector>

#include "engine/screen.h"
#include "engine/sdl_resource.h"
#include "widgets/button.h"

struct MenuButton {
    Button button;
    std::function<ScreenCommand()> on_click;
};

class TitleScreen : public Screen {
   public:
    void on_enter() override;
    void on_exit() override;
    ScreenCommand on_event(const SDL_Event& e) override;
    void on_update(float dt) override;
    void on_render(SDL_Renderer* r) override;

    bool init(SDL_Renderer* r) override;

   private:
    void render_fog_layer(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                          SDL_RendererFlip flip);
    void render_fog_layer_right(SDL_Renderer* r, float x, int y, int w, int h, Uint8 alpha,
                                SDL_RendererFlip flip);

   private:
    TexturePtr _background{nullptr, SDL_DestroyTexture};
    TexturePtr _fog{nullptr, SDL_DestroyTexture};

    std::vector<MenuButton> _buttons;

    float _fogX_1 = 0.0f;
    float _fogX_2 = 0.0f;
    float _fogX_3 = 0.0f;
    float _fogTime = 0.0f;
    float _fogSpeed_1 = 15.0f;
    float _fogSpeed_2 = 30.0f;
    float _fogSpeed_3 = 8.0f;
};

#endif
