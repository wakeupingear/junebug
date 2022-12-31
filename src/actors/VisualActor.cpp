#include "Actors.h"
#include "Sprite.h"
#include "Game.h"
#include "Rendering.h"

using namespace junebug;

VisualActor::VisualActor(Vec2<float> pos) : PureActor()
{
    SetPosition(pos);
}
VisualActor::VisualActor(Vec2<int> pos) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y))
{
}

VisualActor::VisualActor(Vec2<float> pos, std::string imagePath) : PureActor()
{
    SetPosition(pos);
    SetSprite(imagePath);
}
VisualActor::VisualActor(Vec2<int> pos, std::string imagePath) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y), imagePath)
{
}

void VisualActor::SetSprite(std::string imagePath)
{
    if (imagePath == "")
    {
        mSpritePath = "";
        return;
    }

    Game *game = Game::Get();
    if (game)
        mSpritePath = game->GetAssetPaths().sprites + imagePath;
}
Sprite *VisualActor::GetSprite()
{
    return LoadSprite(mSpritePath);
}
std::string VisualActor::GetSpriteName()
{
    return mSpritePath;
}
Vec2<int> VisualActor::GetSpriteSize()
{
    Sprite *sprite = GetSprite();
    if (!sprite)
        return Vec2<int>::Zero;
    return sprite->GetTexSize();
}
Vec2<float> VisualActor::GetActorSize()
{
    Sprite *sprite = GetSprite();
    if (!sprite)
        return Vec2<float>::Zero;
    return mScale * sprite->GetTexSize();
}

void VisualActor::SetOrigin(SpriteOrigin origin, const Vec2<int> &offset)
{
    Sprite *sprite = GetSprite();
    if (!sprite)
        return;
    sprite->SetOrigin(origin, offset);
}

void VisualActor::SetPosition(const Vec2<float> &pos)
{
    mPosition = pos;
}
void VisualActor::MovePosition(const Vec2<float> &pos)
{
    mPosition += pos;
}
Vec2<float> VisualActor::GetPosition() const
{
    return mPosition;
}
Vec2<float> VisualActor::GetStartPosition() const
{
    return mStartPosition;
}

void VisualActor::ClampPosition(const Vec2<float> &min, const Vec2<float> &max)
{
    mPosition.x = std::max(min.x, std::min(mPosition.x, max.x));
    mPosition.y = std::max(min.y, std::min(mPosition.y, max.y));
}

void VisualActor::SetRotation(float rot)
{
    mRotation = rot;
}
float VisualActor::GetRotation() const
{
    return mRotation;
}

void VisualActor::SetAlpha(float alpha)
{
    mColor.a = alpha * 255;
}
void VisualActor::SetAlpha(int alpha)
{
    mColor.a = alpha;
}
int VisualActor::GetAlpha()
{
    return mColor.a;
}

void VisualActor::SetScale(const Vec2<float> &scale)
{
    mScale = scale;
}
void VisualActor::SetScale(float scale)
{
    mScale.x = scale;
    mScale.y = scale;
}
void VisualActor::SetScale(float scaleX, float scaleY)
{
    mScale.x = scaleX;
    mScale.y = scaleY;
}
Vec2<float> VisualActor::GetScale() const
{
    return mScale;
}

void VisualActor::SetColor(const Color &color)
{
    mColor = color;
}
Color VisualActor::GetColor() const
{
    return mColor;
}

void VisualActor::InternalFirstUpdate(float dt)
{
    mStartPosition = mPosition;
}

void VisualActor::Draw()
{
    DrawSprite(mSpritePath, 0, mPosition, {mScale, mRotation, mColor, mRoundToCamera});
}