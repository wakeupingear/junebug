#include "Utils.h"

namespace junebug
{
    SDL_Texture *GetRenderTexture(Vector2 size, SDL_Renderer *renderer, SDL_Texture *texture, bool enableForRenderer)
    {
        int texW, texH;
        if (texture)
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

        if (!texture || (texW != (int)size.x || texH != (int)size.y))
        {
            if (texture)
                SDL_DestroyTexture(texture);
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.x, (int)size.y);
        }

        if (enableForRenderer)
        {
            SDL_SetRenderTarget(renderer, texture);
            SDL_RenderClear(renderer);
        }

        return texture;
    }
}