#include "Game.h"
#include "Sprite.h"

#include <algorithm>

using namespace junebug;

SDL_Texture *Game::GetTexture(std::string fileName)
{
    auto it = mTextures.find(fileName);
    if (it != mTextures.end())
        return it->second;

    SDL_Surface *surface = IMG_Load(fileName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
        PrintLog("Failed to load", "'" + fileName + "'");
    }

    mTextures.insert(std::make_pair(fileName, texture));
    return texture;
}