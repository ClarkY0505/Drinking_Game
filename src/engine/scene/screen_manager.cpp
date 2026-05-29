/*******************************************
 * Screen Manager
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/

#include "engine/screen_manager.h"

void ScreenManager::push(std::unique_ptr<Screen> s)
{
    s->on_enter();
    m_screens.push(std::move(s));
}

void ScreenManager::pop()
{
    if (m_screens.empty()) {
        return;
    }

    m_screens.top()->on_exit();
    m_screens.pop();
}

Screen* ScreenManager::current() 
{ 
    if (m_screens.empty()) {
        return nullptr;
    }

    return m_screens.top().get(); 
}

bool ScreenManager::empty() const
{
    return m_screens.empty();
}
