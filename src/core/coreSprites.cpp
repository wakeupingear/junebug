#include "JGame.h"
#include "Sprite.h"

#include <algorithm>

using namespace junebug;

void JGame::AddSprite(Sprite *sprite)
{
    mSprites.push_back(sprite);
    std::sort(mSprites.begin(), mSprites.end(),
              [](Sprite *a, Sprite *b)
              {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });
}

void JGame::RemoveSprite(Sprite *sprite)
{
    auto loc = std::find(mSprites.begin(), mSprites.end(), sprite);
    if (loc != mSprites.end())
        mSprites.erase(loc);
}

SDL_Texture *JGame::GetTexture(std::string fileName)
{
    auto it = mTextures.find(fileName);
    if (it != mTextures.end())
        return it->second;

    Print("Loading image: " + fileName);
    SDL_Surface *surface = IMG_Load(fileName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
        std::string errorMsg = "Failed to load '" + fileName + "'";
        SDL_Log("%s", errorMsg.c_str());
    }

    mTextures.insert(std::make_pair(fileName, texture));
    return texture;
}

const std::vector<Sprite *> &JGame::GetSprites() const
{
    return mSprites;
}