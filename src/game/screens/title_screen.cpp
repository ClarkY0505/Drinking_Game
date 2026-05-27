/*******************************************
 * Title screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#include <iostream>
#include "engine/title_screen.h"

void TitleScreen::on_enter()
{
    std::cout << "Enter Menu" << std::endl;
}

void TitleScreen::on_exit()
{
    std::cout << "Exit Menu" << std::endl;
}

void TitleScreen::on_event (const SDL_Event& e)
{
    if(SDL_KEYDOWN == e.type){
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

void TitleScreen::on_update(float dt)
{
    (void)dt;
}

void TitleScreen::on_render(SDL_Renderer* r)
{
    SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
    SDL_RenderClear(r);
}
