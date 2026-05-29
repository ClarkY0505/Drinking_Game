/*******************************************
 * Game Main Screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#ifndef DRINKING_GAME_UI_GAME_SCREEN_H
#define DRINKING_GAME_UI_GAME_SCREEN_H

#include "engine/screen.h"

class GameScreen : public Screen{
public:
    void on_enter() override;
    void on_exit() override;
    void on_event(const SDL_Event& e) override;
    void on_update(float dt) override;
    void on_render(SDL_Renderer* r) override;

    bool init(SDL_Renderer* r) override;
 };

#endif
