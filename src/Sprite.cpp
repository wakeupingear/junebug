#include "Sprite.h"
#include "Actors.h"
#include "Game.h"
#include "Camera.h"

using namespace junebug;

Sprite::Sprite()
    : mTexture(nullptr), mTexWidth(0), mTexHeight(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Draw(Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const SpriteProperties &properties)
{
    if (mTexture && cam)
    {
        SDL_Rect r;
        r.w = static_cast<int>(mTexWidth * properties.scale.x * cam->GetZoom());
        r.h = static_cast<int>(mTexHeight * properties.scale.y * cam->GetZoom());

        Vec2<float> dPos = GetDrawPosition(pos - Vec2(r.w, r.h) * 0.5f);
        r.x = static_cast<int>(dPos.x);
        r.y = static_cast<int>(dPos.y);

        SDL_SetTextureColorMod(mTexture, properties.color.r, properties.color.g, properties.color.b);
        SDL_SetTextureAlphaMod(mTexture, properties.color.a);
        SDL_RenderCopyEx(renderer,
                         mTexture,
                         nullptr,
                         &r,
                         -properties.rotation,
                         nullptr,
                         SDL_FLIP_NONE);
    }
}

void Sprite::SetTexture(SDL_Texture *texture)
{
    mTexture = texture;
    // Set width/height
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void Sprite::AddAnimation(const std::string &name,
                          const std::vector<SDL_Texture *> &images)
{
    mAnims.emplace(name, images);
}