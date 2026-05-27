/*******************************************
 * Game Main Screen
 * email  : umzoenda@gmail.com
 * autior : Florian
 * date   : 2026-5-26
 * Copyright (c) 2026
 * All rights reserved.
 *******************************************/
#include <iostream>
#include "engine/game_screen.h"

void GameScreen::on_enter()
{
    std::cout << "Enter Game" << std::endl;
}

void GameScreen::on_exit()
{
    std::cout << "Exit Game" << std::endl;
}

void GameScreen::on_event (const SDL_Event& e)
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
        std::cout << "Game Key Press" << std::endl;
    }
}

void GameScreen::on_update(float dt)
{
    (void)dt;
}

void GameScreen::on_render(SDL_Renderer* r)
{
    SDL_SetRenderDrawColor(r, 50, 50, 240, 255);
    SDL_RenderClear(r);
}
