/*******************************************
 * Screen Manager
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#ifndef DRINKING_GAME_UI_SCREEN_MANAGER_H
#define DRINKING_GAME_UI_SCREEN_MANAGER_H

#include <stack>
#include <SDL2/SDL.h>
#include <memory>
#include "screen.h"

class ScreenManager
{
    std::stack<std::unique_ptr<Screen>> m_screens;

public:

    void push(std::unique_ptr<Screen> s);
    void pop();
    Screen *current();
    bool empty() const;
};

#endif
