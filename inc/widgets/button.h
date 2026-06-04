/*******************************************
 * Button UI & EVENT
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-6-1
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#ifndef DRINKING_GAME_UI_BUTTON_H
#define DRINKING_GAME_UI_BUTTON_H
#include <SDL_rect.h>

class Button {
public:
    SDL_Rect rect;

    bool is_clicked(int x, int y);
};

#endif