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
    if (mTexture)
    {
        Vec2<float> camPos = (cam != nullptr) ? cam->GetPosition() : Vec2<float>::Zero;

        SDL_Rect r;
        r.w = static_cast<int>(mTexWidth * properties.scale.x);
        r.h = static_cast<int>(mTexHeight * properties.scale.y);
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(pos.x - r.w / 2 - camPos.x);
        r.y = static_cast<int>(pos.y - r.h / 2 - camPos.y);

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