#include "Sprite.h"
#include "Actors.h"
#include "JGame.h"
#include "Camera.h"

using namespace junebug;

Sprite::Sprite()
    : mTexture(nullptr), mTexWidth(0), mTexHeight(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Draw(Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const Vec2<float> &size, const float rotation, Color color)
{
    if (mTexture)
    {
        Vec2<float> camPos = (cam != nullptr) ? cam->pos : Vec2<float>::Zero;

        SDL_Rect r;
        r.w = static_cast<int>(mTexWidth * size.x);
        r.h = static_cast<int>(mTexHeight * size.y);
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(pos.x - r.w / 2 - camPos.x);
        r.y = static_cast<int>(pos.y - r.h / 2 - camPos.y);

        // Draw (have to convert angle from radians to degrees, and clockwise to counter)
        SDL_SetTextureColorMod(mTexture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(mTexture, color.a);
        SDL_RenderCopyEx(renderer,
                         mTexture,
                         nullptr,
                         &r,
                         -rotation,
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