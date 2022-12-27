#include "Sprite.h"
#include "Actors.h"
#include "Game.h"
#include "Camera.h"

using namespace junebug;

Sprite::Sprite()
    : mTexture(nullptr)
{
}

Sprite::~Sprite()
{
}

void Sprite::SetOrigin(SpriteOrigin origin, const Vec2<int> &offset)
{
    switch (origin)
    {
    case SpriteOrigin::TopLeft:
        mOrigin = Vec2<int>::Zero;
        break;
    case SpriteOrigin::TopRight:
        mOrigin.x = mTexSize.x;
        break;
    case SpriteOrigin::BottomLeft:
        mOrigin.y = mTexSize.y;
        break;
    case SpriteOrigin::BottomRight:
        mOrigin = mTexSize;
        break;
    case SpriteOrigin::Center:
        mOrigin = mTexSize / 2;
        break;
    }

    mOrigin += offset;
}

void Sprite::Draw(Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const Vec2<int> &partPos, const Vec2<int> &partSize, const SpriteProperties &properties)
{
    if (mTexture && cam)
    {
        SDL_Rect src;
        src.w = static_cast<int>(partSize.x);
        src.h = static_cast<int>(partSize.y);
        src.x = static_cast<int>(partPos.x);
        src.y = static_cast<int>(partPos.y);

        SDL_Rect dest;
        dest.w = static_cast<int>(partSize.x * properties.scale.x * cam->GetZoom());
        dest.h = static_cast<int>(partSize.y * properties.scale.y * cam->GetZoom());

        Vec2<float> dPos = GetDrawPosition(pos - mOrigin * properties.scale);
        if (properties.roundToCamera)
        {
            dPos.x = round(dPos.x);
            dPos.y = round(dPos.y);
        }
        dest.x = static_cast<int>(dPos.x);
        dest.y = static_cast<int>(dPos.y);

        SDL_SetTextureColorMod(mTexture, properties.color.r, properties.color.g, properties.color.b);
        SDL_SetTextureAlphaMod(mTexture, properties.color.a);
        SDL_RenderCopyEx(renderer,
                         mTexture,
                         &src,
                         &dest,
                         -properties.rotation,
                         nullptr,
                         SDL_FLIP_NONE);
    }
}

void Sprite::SetTexture(SDL_Texture *texture)
{
    mTexture = texture;
    // Set width/height
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexSize.x, &mTexSize.y);
}

void Sprite::AddAnimation(const std::string &name,
                          const std::vector<SDL_Texture *> &images)
{
    mAnims.emplace(name, images);
}