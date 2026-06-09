#ifndef DRINKING_GAME_ENGINE_SDL_RESOURCE_H
#define DRINKING_GAME_ENGINE_SDL_RESOURCE_H

#include <SDL2/SDL_render.h>

#include <memory>

using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

inline TexturePtr make_texture_ptr(SDL_Texture* texture) {
    return TexturePtr(texture, SDL_DestroyTexture);
}

#endif
