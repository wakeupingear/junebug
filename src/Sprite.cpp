#include "Sprite.h"
#include "Actors.h"
#include "JGame.h"
#include "Camera.h"

using namespace junebug;

Sprite::Sprite(VisualActor *owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
    JGame::Get()->AddSprite(this);
}

Sprite::~Sprite()
{
    JGame::Get()->RemoveSprite(this);
}

void Sprite::Draw(Camera *cam, SDL_Renderer *renderer)
{
    if (mTexture && IsVisible())
    {
        Vec2<float> camPos = cam->pos;

        SDL_Rect r;
        r.w = static_cast<int>(mTexWidth * mOwner->GetScale().x);
        r.h = static_cast<int>(mTexHeight * mOwner->GetScale().y);
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 - camPos.x);
        r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 - camPos.y);

        // Draw (have to convert angle from radians to degrees, and clockwise to counter)

        SDL_RenderCopyEx(renderer,
                         mTexture,
                         nullptr,
                         &r,
                         -mOwner->GetRotation(),
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

void Sprite::Update(float deltaTime)
{
    if (mAnimName.empty())
        return;

    if (!mIsPaused)
    {
        auto anim = mAnims[mAnimName];
        mAnimTimer += mAnimFPS * deltaTime;
        while (mAnimTimer >= anim.size())
            mAnimTimer -= anim.size();
        SetTexture(anim[(int)(mAnimTimer)]);
    }
}

void Sprite::AddAnimation(const std::string &name,
                          const std::vector<SDL_Texture *> &images)
{
    mAnims.emplace(name, images);
}